#pragma once
#include "../../stdlib/string.cpp"
#include "../../drivers/video/video.cpp"
#include "../sys/power/shutdown.cpp"
#include "../sys/time/timer.h"
#include "../fs/fs.h"
#include "../sys/power/reboot.cpp"
#include "../sys/log/log.h"

void rc(char * b)
{
    if (startswith(b, "echo")) {

        for (int z = 5; z < bf; z++) {
            putchar(b[z]);
        }

    } else if (startswith(b, "time")) {

        printf("Date: %s %s %d\n", weekdays[weekday-1], months[month-1], day);
        printf("Time: %d:%d", hour, minute);

    } else if (startswith(b, "help")) {

        printf("CeneOS 1.0, Daniyal Warraich\n");
        printf("Available commands:\n    - echo\n    - shutdown_os\n    - clear");

    } else if (startswith(b, "ls")) {

        int count = ls_file();
        printf("Number of files: %d", count);

        if (fs.file_number > 0) {
            printf("\nFiles: \n");

            for (int z = 0; z < file_limit; z++)
            {
                if (fs.files[z].name != "NULL")
                    printf("    %s\n", fs.files[z].name);
            }
        }

    } else if (startswith(b, "reboot")) {

        reboot();

    } else if (startswith(b, "mkfile")) {

        char filename[len(b)-7];
        int fz = 0;

        for (int z = 7; z < len(b); z++) {
            if (b[z] != ' ')
            {
                filename[fz] = b[z];
                fz++;
            }
        }

        create_file(filename);

        printf("File %s successfully created!", filename);

    } else if (startswith(b, "copyright")) {

        printf("CeneOS 1.0, Daniyal Warraich 2021");

    } else if (startswith(b, "viewlog")) {

        char nl[sys_log_size + 1];

        for (int z = 0; z < sys_log_size; z++)
        {
            nl[z] = sys_log[z];
        }

        nl[sys_log_size] = '\0';

        printf(nl);

    } else if (startswith(b, "cpuinfo")) {

        unsigned short mem = get_available_memory();
        int mb;
        char * m;
        mb = get_mb(mem);
        itoa(mb, m);
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

    } else if (startswith(b, "shutdown")) {

        disable_interrupts();
        shutdown_os();

    } else if (startswith(b, "cls") || startswith(b, "clear")) {

        clear();
        text_y = -2;

    } else {

        if (strisempty(b)) {
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
