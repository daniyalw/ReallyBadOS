#pragma once
#include <string.h>
#include <system.h>
#include <sys/time/time.h>
#include <shell/shell.h>
#include <video/video.h>
#include <keyboard.h>
#include <sys/mem/memory.h>
#include <fs/fs.h>

using namespace std;
using namespace Time;

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

void run_command(char * command)
{
    if (has_scrolled)
        printf("\n");

    else if (check_name(command, "echo"))
    {
        for (int z = 5; z < strlen(command); z++)
            putchar(command[z]);

        #ifdef DEBUG
        for (int z = 5; z < strlen(command); z++)
            Kernel::system_log_char(command[z]);
        #endif
    }
    else if (check_name(command, "info"))
    {
        printf("OS: %s\n", System::SYSTEM);
        printf("Version: %s\n", System::VERSION);
        printf("Copyright Daniyal Warraich 2022\n");

        int total = total_memory/1048576;
        int times_done = 0;

        char *result[] = {
            "MB",
            "GB",
            "TB",
            "PB",
            "EB",
            "ZB",
            "YB"
        };

        while (total >= 1000)
        {
            total = total/1000;
            times_done++;
        }

        printf("Total memory: %d %s\n", total, result[times_done]);
    }
    else if (check_name(command, "time"))
    {
        // time
        time_t time = get_time();
        if (time.min < 10)
            printf("%d:0%d ", time.h, time.min);
        else
            printf("%d:%d ", time.h, time.min);
        printf((char *)(time.pm ? "PM" : "AM"));
        printf("\n");
    }
    else if (check_name(command, "date"))
    {
        time_t time = get_time();
        printf("%s, %s %d", weekdays[time.wd-1], months[time.m-1], time.d);
    }
    else if (check_name(command, "ls"))
    {
        if (current_display_len == 0 || current_display_len == 1)
            ls("/");
        else
            ls(current_display);
    }
    else if (check_name(command, "exit") || check_name(command, "shutdown") || check_name(command, "quit"))
    {
        Kernel::shutdown_os();
    }
    else if (check_name(command, "help"))
    {
        for (int z = 0; z < 20; z += 2)
        {
            printf("%s: %s\n", commands[z], commands[z+1]);
        }
    }
    else
    {
        if (!strcmp(command, ""))
            printf("Error: command '%s' not found!\n", command);
        else
            printf("Nothing...");
    }
}

void execute_script(script_t script)
{
    const int length = strlen(script.commands);
    char * command = (char *)malloc(length);
    int sz = 0;

    for (int z = 0; z < length; z++)
    {
        if (script.commands[z] == '\n')
        {
            run_command(command);

            for (int b = 0; b < sz; b++)
                command[b] = 0;

            sz = 0;
        }
        else
        {
            command[sz] = script.commands[z];
            sz++;
            command[sz] = 0;
        }
    }
}

void shell()
{
    while (true)
    {
        if (current_display_len != 0 && current_display_len != 1)
            printf("/%s/> ", current_display);
        else
            printf("/> ");

        char * command = scanf();

        if (!command)
        {
#ifdef DEBUG
            Kernel::system_log("String is NULL.\n");
#endif
            printf("\n\n");
            continue;
        }

        run_command(command);

        printf("\n\n");

        free(command); // since scanf() uses malloc()
    }
}
