#include <stdio.h>
#include <filesystem.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int ret = printf("AAAAB");
    printf("Ret: %d\n", ret);

    char *c = getenv("PATH");

    printf("PATH from getenv(): %s\n", c);

    printf("Setting setenv() temp: %d\n", setenv("PATH", "SHIT", 0));
    printf("PATH from getenv(): %s\n", getenv("PATH"));

    return 0;
}
