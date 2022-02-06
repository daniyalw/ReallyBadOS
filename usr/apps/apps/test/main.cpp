#include <stdio.h>

int main(int argc, char const *argv[])
{
    printf("Test printf!\n");

    char *result = scanf();

    printf("Read from scanf(): %s\n", result);

    char k = getch();

    printf("Read from getch(): %c\n", k);

    return 0;
}
