#include <stdint.h>
#include <stdio.h>
#include <file.h>
#include <stdlib.h>

extern "C" char getch()
{
    // read files
    FILE *file = fopen("/dev/stdin", "r");

    if (file == NULL || !file->node || !file->node->read)
        return NULL;

    char c;

    file->node->read(file->node, 0, 100, &c);

    fclose(file);

    return c;
}

extern "C" char *scanf(char *buf)
{
    // TODO parse parameter
    (void)(buf);
    FILE *file = fopen("/dev/stdin", "r");

    if (file == NULL || !file->node || !file->node->read)
        return NULL;

    file->node->read(file->node, 0, 100, buf);

    fclose(file);

    return buf;
}
