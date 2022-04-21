#include <stdio.h>

int main(int argc, char **argv)
{
    for (int z = 1; z < argc; z++)
    {
        text::write("%s ", argv[z]);
    }

    return 0;
}
