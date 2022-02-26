#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getlength(int start, int argc, char *args[])
{
    int length = 0;

    for (int z = start; z < argc; z++)
        length += strlen(args[z]);

    return length;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Help: write <file-path> <contents>");
        return 1;
    }

    const int length = getlength(2, argc, argv);

    char contents[length];
    int cz = 4;
    memset(contents, 0, length);

    for (int z = 2; z < argc; z++)
    {
        for (int b = 0; b < strlen(argv[z]); b++)
        {
            contents[cz] = argv[z][b];
            cz++;
        }

        contents[cz] = ' ';
        cz++;
    }

    contents[cz] = 0;

    char *c = (char *)&contents[4];

    printf("To write: '%s'", c);

    FILE *file = fopen(argv[1]);
    file->write(c);
    fclose(file);

    return 0;
}
