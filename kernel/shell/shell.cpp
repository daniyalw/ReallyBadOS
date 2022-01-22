#pragma once
#include <string.h>
#include <system.h>
#include <time.h>
#include <kernel/shell.h>

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
    if (check_name(command, "die"))
    {
        printf("\nwhy so mean?");
    }
    else if (check_name(command, "echo"))
    {
        printf("\n");
        for (int z = 5; z < strlen(command); z++)
            putchar(command[z]);
    }
    else if (check_name(command, "info"))
    {
        printf("\nOS: %s\n", System::SYSTEM);
        printf("Version: %s\n", System::VERSION);
    }
    else if (check_name(command, "time"))
    {
        printf("\n");

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
        printf("\n%s, %s %d", weekdays[time.wd-1], months[time.m-1], time.d);
    }
    else if (check_name(command, "ls"))
    {
        ls("/");
    }
    else if (check_name(command, "exit") || check_name(command, "shutdown") || check_name(command, "quit"))
    {
        Kernel::shutdown_os();
    }
    else if (check_name(command, "help"))
    {
        for (int z = 0; z < 20; z += 2)
        {
            printf("\n%s: %s\n", commands[z], commands[z+1]);
        }
    }
    else
    {
        if (!strcmp(command, ""))
            printf("\nError: command '%s' not found!\n", command);
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
        printf("/> ");

        char * command = scanf();

        if (!command)
        {
#ifdef DEBUG
            printf("String is NULL.\n");
#endif
            printf("\n\n");
            continue;
        }

        run_command(command);

        printf("\n\n");

        free(command); // since scanf() uses malloc()
    }
}
