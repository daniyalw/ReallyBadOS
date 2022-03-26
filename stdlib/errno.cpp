#include <errno.h>

char *strerror(int err) {
    return error_strings[err - 1];
}

void perror(char *func) {
    printf("%s: %s\n", func, strerror(errno));
}
