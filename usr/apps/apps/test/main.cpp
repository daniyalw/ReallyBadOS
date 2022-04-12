#include <stdio.h>
#include <filesystem.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int ret = printf("AAAAB");
    printf("Ret: %d\n", ret);

    printf("PATH from getenv(): %s\n", getenv("PATH"));

    return 0;
}
