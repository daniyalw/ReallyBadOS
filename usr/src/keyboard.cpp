#include <stdint.h>
#include <stdio.h>
#include <file.h>
#include <stdlib.h>

extern "C" char getch()
{
    // read files
    FILE *file = fopen("/dev/stdin", "r");

    if (file == NULL)
        return NULL;

    char buf[100];

    file->node->read(file->node, 0, 100, buf);

    fclose(file);

    return buf[0];
}

extern "C" char *scanf(char *buf)
{
    // TODO parse parameter
    (void)(buf);
    FILE *file = fopen("/dev/stdin", "r");

    if (file == NULL)
        return NULL;

    char *out = (char *)malloc(100);
    memset(out, 0, 100);

    file->node->read(file->node, 0, 100, out);

    fclose(file);

    return out;
}
