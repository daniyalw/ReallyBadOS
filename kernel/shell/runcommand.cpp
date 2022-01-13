#pragma once
#include <string.h>
#include <stdio.h>
#include <kernel/power.h>
#include <timer.h>
#include <kernel/log.h>
#include <sys.h>
#include <fs.h>
#include "rc.h"

using namespace std;
using namespace Filesystem;
using namespace Ramdisk;

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

void rc(char * b)
{
    if (check_name(b, "echo")) {

        for (int z = 5; z < bf; z++) {
            putchar(b[z]);
        }
    } else if (check_name(b, "ls")) {

        Filesystem::ls();

    } /*else if (check_name(b, "mkdir")) {

        int length = len(b);
        char name[length];
        int c = 0;

        for (int z = 0; z < length; z++)
            name[z] = 0;

        for (int z = 6; z < length; z++)
        {
            name[c] = b[z];
            c++;
        }

        create_folder(name);

    } */else if (check_name(b, "cp")) {

        int length = len(b);
        char name[length];
        int c = 0;

        for (int z = 0; z < length; z++)
            name[z] = 0;

        for (int z = 3; z < length; z++)
        {
            name[c] = b[z];
            c++;
        }

        //copy_file(name)

    } else if (check_name(b, "read")) {

        int length = len(b);
        char name[length];
        int c = 0;

        for (int z = 0; z < length; z++)
            name[z] = 0;

        for (int z = 5; z < length; z++)
        {
            name[c] = b[z];
            c++;
        }

        FILE file = get_file(name, "usr");

        if (!file) {
            printf("Error: '%s' not valid!\n", name);
        } else {
            printf("Name: %s\nSize: %d bytes\nContents: %s\n", file.name, file.size, file.contents);
        }

    } else if (check_name(b, "cd")) {

        int length = len(b);
        char directory[length];
        int c = 0;

        for (int z = 0; z < length; z++)
            directory[z] = 0;

        for (int z = 3; z < length; z++)
        {
            directory[c] = b[z];
            c++;
        }

        if (strcmp(directory, ".."))
        {
            for (int z = 0; z < 128; z++)
                current_display[z] = 0;

            printf("Moved to %s\n", directory);
            return;
        }

        //printf("You want to go to directory %s?\n", directory);

        FOLDER folder = get_folder(directory);

        if (folder.null)
        {
            printf("Error: no such directory!\n");
        }
        else
        {
            for (int z = 0; z < 128; z++)
                current_display[z] = 0;
            int dir_len = std::len(directory);
            ASSERT(dir_len < 128);
            for (int z = 0; z < dir_len; z++)
                current_display[z] = directory[z];
            current_display_len = dir_len;
            printf("Moved to %s\n", directory);
        }

    } else if (check_name(b, "time")) {

        printf("Date: %s %s %d\n", weekdays[weekday-1], months[month-1], day);
        if (hour > 12) {
            printf("Time: %d:%d PM", hour - 12, minute);
        } else {
            printf("Time: %d:%d AM", hour, minute);
        }

    } else if (check_name(b, "help")) {

        printf("%s %s, Daniyal Warraich\n", System::SYSTEM, System::VERSION);
        printf("Available commands:\n");

    } else if (check_name(b, "about")) {

        printf("%s %s, Daniyal Warraich", System::SYSTEM, System::VERSION);

    } else if (check_name(b, "reboot")) {

        Kernel::reboot();

    } else if (check_name(b, "copyright")) {

        printf("%s %s, Daniyal Warraich 2021", System::SYSTEM, System::VERSION);

    } else if (check_name(b, "cpuinfo")) {

        unsigned short mem = get_available_memory();
        int mb;
        char * m;
        printf("Approximate Memory: ");
        printf(m);
        char * cpuname = get_cpu_name();
        printf(" MB\nCPU name: ");
        printf(cpuname);
        char * vendor = get_vendor();
        printf("\nVendor: ");
        printf(vendor);
        int model = get_model();
        printf("\nModel: ");
        char * mdl;
        itoa(model, mdl);
        printf(mdl);

    } else if (check_name(b, "shutdown")) {

        printf("Shutting down...\n");
        Kernel::disable_interrupts();
        Kernel::shutdown_os();

    } else if (check_name(b, "cls") || check_name(b, "clear")) {

        clear();
        text_y = -2;

    } else {

        if (strisempty(b)) {
            text_y--;
            return;
        }

        if (strip(b, ' ') == "") {
            return;
        }

        printf("Error: command not found.");
        printf("\nCommand: ");
        for (int z = 0; z < bf; z++) {
            putchar(b[z]);
        }

    }

}
