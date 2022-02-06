#include <stdio.h>
#include <system.h>

int main(int argc, char const *argv[])
{
    info_t *info = get_sys_info();

    printf("OS: %s\n", info->name);
    printf("Version: %s\n", info->version);

    return 0;
}
