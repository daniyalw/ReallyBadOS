#pragma once
#include <string.h>

using namespace std;

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

void shell()
{
    while (true)
    {
        printf("\n\n/> ");
        char * command = scanf();

        if (check_name(command, "die"))
        {
            printf("yay");
        }
        else if (check_name(command, "echo"))
        {
            for (int z = 5; z < strlen(command); z++)
                putchar(command[z]);
        }
        else
        {
            printf("Error: command '%s' not found!\n", command);
        }

        free(command); // since scanf() uses malloc()
    }
}
