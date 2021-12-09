#pragma once
#include <string.h>
#include <stdio.h>
#include <kernel/power.h>
#include <timer.h>
#include <kernel/log.h>
#include <sys.h>
#include "rc.h"

void rc(char * b)
{
    if (std::startswith(b, "echo")) {

        for (int z = 5; z < bf; z++) {
            putchar(b[z]);
        }

    } else if (std::startswith(b, "time")) {

        printf("Date: %s %s %d\n", weekdays[weekday-1], months[month-1], day);
        if (hour > 12) {
            printf("Time: %d:%d PM", hour - 12, minute);
        } else {
            printf("Time: %d:%d AM", hour, minute);
        }

    } else if (std::startswith(b, "help")) {

        printf("%s %s, Daniyal Warraich\n", System::SYSTEM, System::VERSION);
        printf("Available commands:\n");

    } else if (std::startswith(b, "about")) {

        printf("%s %s, Daniyal Warraich", System::SYSTEM, System::VERSION);

    } else if (std::startswith(b, "reboot")) {

        Kernel::reboot();

    } else if (std::startswith(b, "copyright")) {

        printf("%s %s, Daniyal Warraich 2021", System::SYSTEM, System::VERSION);

    } else if (std::startswith(b, "cpuinfo")) {

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
        std::itoa(model, mdl);
        printf(mdl);

    } else if (std::startswith(b, "shutdown")) {

        printf("Shutting down...\n");
        disable_interrupts();
        Kernel::shutdown_os();

    } else if (std::startswith(b, "cls") || std::startswith(b, "clear")) {

        clear();
        text_y = -2;

    } else {

        if (std::strisempty(b)) {
            text_y--;
            return;
        }
        printf("Error: command not found.");
        printf("\nCommand: ");
        for (int z = 0; z < bf; z++) {
            putchar(b[z]);
        }

    }

}
