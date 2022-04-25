#include <stdio.h>
#include <filesystem.h>
#include <stdlib.h>

void show_err(char *operation) {
    printf("An error occured while trying to %s file.\n", operation);
}

int main(int argc, char *argv[])
{
    auto file = Filesystem::open("/dev/stdout", "r");

    if (!file) {
        show_err("open");
        return 1;
    }

    int ret = fprintf(file, "Hello, world!");

    printf("HSAA");
    return ret;
}
