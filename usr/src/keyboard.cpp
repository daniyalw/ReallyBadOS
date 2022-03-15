#include <stdint.h>
#include <stdio.h>
#include <file.h>
#include <stdlib.h>

extern "C" char getch()
{
    // read files
    FILE *file = fopen("/dev/stdin");

    if (file == NULL)
        return NULL;

    char buf[100];

    file->node->read(file->node, 0, 100, buf);

    fclose(file);

    return buf[0];
}

extern "C" char *scanf()
{
    FILE *file = fopen("/dev/stdin");
    char *buf = (char *)malloc(100);

    if (file == NULL)
        return NULL;

    file->node->read(file->node, 0, 100, buf);

    fclose(file);

    return buf;
}
