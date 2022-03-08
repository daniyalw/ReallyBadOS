#include <stdint.h>
#include <stdio.h>
#include <file.h>

extern "C" char getch()
{
    // read files
    FILE *file = fopen("/dev/stdin"); // dunno why but getch doesn't work
    char *buff;
    buff = fread(buff, 0, 100, file);


    fclose(file);

    return buff[0];
}

extern "C" char *scanf()
{
    FILE *file = fopen("/dev/stdin");
    char *buff;

    buff = fread(buff, 0, 1000, file);

    fclose(file);

    return buff;
}
