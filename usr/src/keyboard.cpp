#include <stdint.h>
#include <stdio.h>
#include <file.h>

extern "C" char getch()
{
    FILE *file = fopen("/dev/getch");
    char *buff;
    buff = file->read(buff);

    fclose(file);

    return buff[0];
}

extern "C" char *scanf()
{
    FILE *file = fopen("/dev/stdin");
    char *buff;

    buff = file->read(buff);

    fclose(file);

    return buff;
}
