#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START 2

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Help: append <file-to-append> <contents-to-append>");
        return 1;
    }

    int length = argc;

    for (int z = START; z < argc; z++)
        length += strlen(argv[z]);

    char *contents;
    int cz = 0;

    for (int z = 0; z < length; z++)
    {
        contents[z] = 0;
    }

    for (int z = START; z < argc; z++)
    {
        for (int b = 0; b < strlen(argv[z]); b++)
        {
            contents[cz] = argv[z][b];
            cz++;
        }
    }

    append_file(argv[1], contents);

    return 0;
}
