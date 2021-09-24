int system(char * command)
{
    if (startswith(command, "echo"))
    {
        char * to_print;

        for (int z = 5; z < len(command); z++)
            to_print += command[z];

        printf(to_print);
        return 0;
    }
    else
    {
        char * display = "kernel: command not found: ";
        printf(display);
        printf(command);
        return 1;
    }
}
