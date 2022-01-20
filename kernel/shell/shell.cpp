#pragma once
#include <string.h>
#include <system.h>
#include <time.h>

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

int total_command_count = 20;
char *commands[] = {
    "die",
    "A test command",
    "echo",
    "Print to the screen",
    "info",
    "Get system info",
    "time",
    "Get the time",
    "date",
    "Get the date",
    "ls",
    "List folders and files",
    "exit",
    "Shutdown",
    "shutdown",
    "Shutdown",
    "quit",
    "Shutdown",
    "help",
    "Get list of commands"
};

void shell()
{
    while (true)
    {
        printf("\n\n/> ");

        char * command = scanf();

        if (check_name(command, "die"))
        {
            printf("why so mean?");
        }
        else if (check_name(command, "echo"))
        {
            for (int z = 5; z < strlen(command); z++)
                putchar(command[z]);
        }
        else if (check_name(command, "info"))
        {
            printf("OS: %s\n", System::SYSTEM);
            printf("Version: %s\n", System::VERSION);
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
                printf("%s: %s\n", commands[z], commands[z+1]);
            }
        }
        else
        {
            if (!strcmp(command, ""))
                printf("Error: command '%s' not found!\n", command);
        }

        free(command); // since scanf() uses malloc()
    }
}
