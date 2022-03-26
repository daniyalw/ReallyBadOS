#include <errno.h>

char *strerror(int err) {
    return error_strings[err - 1];
}
