#pragma once
#include "../include/string.cpp"
#include "../drivers/video/video.cpp"
#include "../sys/shutdown/shutdown.cpp"
#include "../fs/fs.cpp"

void rc(char * b)
{
    /*
    Commands:
        - echo
        - shutdown_os
        - cls
        - clear
        - help
        - copyright
        - cpuinfo
    */
    if (startswith(b, "echo")) {

        for (int z = 5; z < bf; z++) {
            putchar(b[z]);
        }

    } else if (startswith(b, "help")) {

        printf("CeneOS 1.0, Daniyal Warraich\n");
        printf("Available commands:\n    - echo\n    - shutdown_os\n    - clear");

    } else if (startswith(b, "copyright")) {

        printf("CeneOS 1.0, Daniyal Warraich 2021");

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

    } else if (startswith(b, "mk")) {

        char n[10];

        create_file(n, "");
        printf("Successfully created file %s!", n);

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
