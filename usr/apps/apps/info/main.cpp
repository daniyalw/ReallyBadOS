#include <stdio.h>
#include <system.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    info_t info = get_sys_info();

    printf("OS: %s\n", info.name);
    printf("Version: %s%s\n", info.version, (char *)(info.dev ? " Dev" : ""));

    return 0;
}
