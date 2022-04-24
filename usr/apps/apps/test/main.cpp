#include <stdio.h>
#include <filesystem.h>
#include <stdlib.h>

void show_err(char *operation) {
    printf("An error occured while trying to %s file.\n", operation);
}

int main(int argc, char *argv[])
{
    auto file = Filesystem::open("/home/guest/documents/hello.txt", "r");

    if (!file) {
        show_err("open");
        return 1;
    }

    char buf[100];
    memset(buf, 0, 100);

    auto ret = Filesystem::read(buf, 1, 100, file);

    if (ret) {
        show_err("read");
        return 1;
    }

    printf("Read: %s\n", buf);

    Filesystem::close(file);
    return 0;
}
