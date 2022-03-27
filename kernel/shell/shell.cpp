#pragma once
#include <string.h>
#include <system.h>
#include <sys/time/time.h>
#include <shell/shell.h>
#include <video/video.h>
#include <keyboard.h>
#include <sys/mem/memory.h>
#include <filesystem/file.h>
#include <exec/argparse.h>
#include <exec/elf.h>
#include <sys/syscall/user.h>
#include <ctype.h>
#include <keyboard/keys.cpp>
#include <kernel/sys/serial.h>
#include <sys/multitasking/task.h>

using namespace std;
using namespace Time;

void print_both(char *text, ...) {
    va_list va;

    va_start(va, text);
    log::info(text, va);
    va_end(va);

    va_start(va, text);
    vprintf(text, va);
    va_end(va);
}

void putchar_both(char text, ...) {
    putchar(text);
    Kernel::output_serial_char(text);
}

bool check_name(char * name, char * check_against) {
    if (!startswith(name, check_against))
        return false;

    if (len(name) == len(check_against))
        if (startswith(name, check_against))
            return true;

    // here the lengths do not match
    if (startswith(name, check_against))
        if (name[len(check_against)] != 0 && name[len(check_against)] != ' ')
            return false;

    return true;
}

int run_command(char * command) {
    args_t args = parse_args(command);
    char *executable = args.argv[0];
    char *fname;
    bool is_exec_slash = false;

    if (startswith(executable, "/")) {
        is_exec_slash = true;
    } else {
        fname = get("", "/bin/%s.o", executable);
    }

    FILE *file;

    if (is_exec_slash)
        file = fopen(executable, "r");
    else
        file = fopen(fname, "r");

    if (file == NULL) {
        fclose(file);

        // if we fail, try searching the usr folder
        fname = get("", "/usr/bin/%s.o", executable, executable);
        file = fopen(fname, "r");

        if (file == NULL) {
            printf("shell: command not found: %s\n", args.argv[0]);
            return 1;
        }
    }
#ifdef DEBUG
    printf("found app!\n");
    printf("Arg count: %d\n", args.argc);
#endif

    char *argv[args.argc];

    for (int z = 0; z < args.argc; z++)
        argv[z] = args.argv[z];

    int ret;

    if (Kernel::CPU::tasking_on) {
        Kernel::CPU::pid_t pid = Kernel::CPU::create_process_file(file, args.argc, argv);

        ret = Kernel::CPU::wait_retcode(pid);
    } else {
        ret = elf_start((uint8_t *)file->node->contents, args.argc, argv);
    }

#ifdef DEBUG
    log::error("Return code: %d\n", ret);
#endif

    //fclose(file);

    return 0;
}

int execute_script(char *text) {
    const int length = strlen(text);
    char * command = (char *)malloc(length);
    for (int z = 0; z < length; z++) command[z] = 0;
    int sz = 0;

    for (int z = 0; z < length; z++) {
        if (text[z] == '\n') {
            if (strisempty(command)) continue;
            int ret = run_command(command);

            if (ret) {
                printf("Error: r: application exited with return code 1\n");
                return 1;
            }

            printf("Returned: %d\n", ret);

            for (int b = 0; b < sz; b++)
                command[b] = 0;

            sz = 0;
        } else {
            command[sz] = text[z];
            sz++;
            command[sz] = 0;
        }
    }

    return 0;
}

void shell_quit() {
    putchar('\n');
    printf("Exited application\n");
    log::info("User exited application using Ctrl+C");
    shell();
}

void shell() {
    while (true) {
        if (current_display_len != 0 && current_display_len != 1) {
            printf("/%s/> ", current_display);
        } else {
            printf("/> ");
        }

        char * command = scanf();

        if (strisempty(command) || command == "") {
            putchar('\n');
            continue;
        }

        if (check_name(command, "exec")) {
            printf("Warning: %s\n", "executable not properly working...\n");
            printf("Do you wish to continue? [y/n] ");

            char res = getch();

            putchar('\n');
            putchar('\n');

            if (res != 'y')
                continue;
        } else if (check_name(command, "exit")) {
            Kernel::CPU::exit(0);
        }

        memset(last_command, NULL, 100);
        strcpy(last_command, command);

        run_command(command);

        putchar('\n');
        putchar('\n');

        free(command); // since scanf() uses malloc()
    }
}
