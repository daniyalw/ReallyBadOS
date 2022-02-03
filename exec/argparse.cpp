#include <exec/argparse.h>

bool there_is_more(char *command, int z)
{
    for (int b = z; b < std::len(command); b++)
        if (command[b] != ' ')
            return true;

    return false;
}

args_t parse_args(char *command)
{
    args_t args;

    for (int z = 0; z < 64; z++)
        for (int b = 0; b < 64; b++)
            args.argv[z][b] = 0;

    int argc = 1;
    const int length = std::len(command);
    char argv[64][64];
    int charcount = 0;
    bool found_char = false;

    // set argv to 0
    for (int z = 0; z < 64; z++)
        for (int b = 0; b < 64; b++)
            argv[z][b] = 0;

    for (int z = 0; z < length; z++)
    {
        if (command[z] == ' ')
        {
            if (!found_char) continue;

            if (there_is_more(command, z))
                argc++;

            charcount = 0;
        }
        else
        {
            found_char = true;
            argv[argc-1][charcount] = command[z];
            charcount++;
        }
    }

    for (int z = 0; z < argc; z++)
        for (int b = 0; b < 64; b++)
            args.argv[z][b] = argv[z][b];

    args.argc = argc;

    return args;
}
