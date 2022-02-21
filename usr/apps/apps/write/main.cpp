#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START 2

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Help: write <file-path> <contents>");
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

    printf("To write: %s\n", contents);

    FILE *file = fopen(argv[1]);

    int result = file->write(contents);

    if (result == 0)
    {
        printf("Successfully wrote to file!\n");
    }
    else
    {
        printf("Failed to write to file %s!\n", argv[1]);
    }

    return 0;
}
