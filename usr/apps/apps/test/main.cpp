#include <stdio.h>
#include <filesystem.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int ret = text::write("AAAAB");
    text::write("Ret: %d\n", ret);

    char *c = getenv("PATH");

    text::write("PATH from getenv(): %s\n", c);

    text::write("Setting setenv() temp: %d\n", setenv("PATH", "SHIT", 0));
    text::write("PATH from getenv(): %s\n", getenv("PATH"));

    return 0;
}
