#include <stdio.h>

int main(int argc, char **argv)
{
    for (int z = 0; z < argc; z++)
    {
        printf("%s ", argv[z]);
    }

    return 0;
}
