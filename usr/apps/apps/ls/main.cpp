#include <stdio.h>
#include <file.h>

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        ls(NULL);
    }
    else
    {
        ls(argv[1]);
    }

    return 0;
}
