void rc(char * b)
{
    /*
    Commands:
        - echo
        - shutdown
        - cls
        - clear
    */
    if (startswith(b, "echo")) {

        for (int z = 5; z < bf; z++) {
            putchar(b[z]);
        }

    } else if (startswith(b, "help")) {

        printf("CeneOS 1.0, Daniyal Warraich\n");
        printf("Available commands:\n    - echo\n    - shutdown\n    - clear");

    } else if (startswith(b, "copyright")) {

        printf("CeneOS 1.0, Daniyal Warraich 2021");

    } else if (startswith(b, "shutdown")) {

        shutdown();

    } else if (startswith(b, "cls") || startswith(b, "clear")) {

        clear();
        text_y = -1;

    } else {

        if (strcheckempty(b)) {
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
