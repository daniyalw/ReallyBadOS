#pragma once
#include <string.h>
#include <system.h>
#include <sys/time/time.h>
#include <shell/shell.h>
#include <video/video.h>
#include <keyboard.h>
#include <sys/mem/memory.h>
#include <fs.h>
#include <exec/argparse.h>
#include <exec/elf.h>
#include <sys/multitasking/cooperative.h>
#include <sys/syscall/user.h>
#include <ctype.h>

using namespace std;
using namespace Time;

void print_both(char *text, ...)
{
    va_list va;

    va_start(va, text);
    Kernel::system_log_string(text, va);
    va_end(va);

    va_start(va, text);
    vprintf(text, va);
    va_end(va);
}

void putchar_both(char text, ...)
{
    putchar(text);
    Kernel::system_log_char(text);
}

bool check_name(char * name, char * check_against)
{
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

int run_command(char * command)
{
    args_t args = parse_args(command);
    char *executable = args.argv[0];
    char *fname = get("", "/apps/%s.o", executable);

    FILE *file = fopen(fname);

    if (file == NULL)
    {
        fclose(file);

        // if we fail, try searching the usr folder
        fname = get("", "/usr/%s.o", executable);
        file = fopen(fname);

        if (file == NULL)
        {
            printf("Error: command not found: %s\n", args.argv[0]);
            return 1;
        }
    }
#ifdef DEBUG
    printf("found app!\n");
    printf("Arg count: %d\n", args.argc);
#endif

    char *argv[args.argc];

    for (int z = 0; z < args.argc; z++)
    {
        argv[z] = args.argv[z];
    }

    int ret = load_app_from_file(file, args.argc, argv);

    fclose(file);

    return ret;
}

int execute_script(char *text)
{
    const int length = strlen(text);
    char * command = (char *)malloc(length);
    for (int z = 0; z < length; z++) command[z] = 0;
    int sz = 0;

    for (int z = 0; z < length; z++)
    {
        if (text[z] == '\n')
        {
            if (strisempty(command)) continue;
            int ret = run_command(command);

            if (ret)
            {
                printf("Error: r: application exited with return code 1\n");
                return 1;
            }

            printf("Returned: %d\n", ret);

            for (int b = 0; b < sz; b++)
                command[b] = 0;

            sz = 0;
        }
        else
        {
            command[sz] = text[z];
            sz++;
            command[sz] = 0;
        }
    }

    return 0;
}

void shell()
{
    while (true)
    {
        if (current_display_len != 0 && current_display_len != 1)
        {
#ifdef DEBUG
            Kernel::system_log("/%s/> ", current_display);
#endif
            printf("/%s/> ", current_display);
        }
        else
        {
#ifdef DEBUG
            Kernel::system_log("/> ");
#endif
            printf("/> ");
        }

        char * command = scanf();

        if (strisempty(command) || command == "")
        {
#ifdef DEBUG
            Kernel::system_log("String is NULL.\n");
#endif
            putchar('\n');
            continue;
        }

#ifdef DEBUG
        Kernel::system_log(command);
#endif
        if (check_name(command, "exec"))
        {
            printf("Warning: %s\n", "executable not properly working...\n");
            printf("Do you wish to continue? [y/n] ");

            char res = getch();

            putchar('\n');
            putchar('\n');

            if (res != 'y')
                continue;
        }
        else if (check_name(command, "exit"))
        {
            printf("Exiting...\n");
            break;
        }

        run_command(command);

        putchar('\n');
        putchar('\n');

        free(command); // since scanf() uses malloc()
    }

    printf("Would you like to shutdown? [y/n] ");
    char result = getch();

    if (result == 'y')
    {
        Kernel::shutdown_os();
    }
}
