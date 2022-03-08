#include <stdio.h>
#include <filesystem.h>

int main(int argc, char *argv[])
{
    printf("Test printf!\n");

    FILE *file = fopen(argv[1]);

    fwrite(argv[2], 0, 10, file);

    fclose(file);

    char *result = scanf();

    printf("Read from scanf(): %s\n", result);;



    char k = getch();

    printf("Read from getch(): %c\n", k);

    return 0;
}
