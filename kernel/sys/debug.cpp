#include "debug.h"

void debug(char *msg, char *file, char *func, int line) {
#if DEBUG_URGENT == 1
    printf("DEBUG: %s:%d, %s\n\t%s\n", file, line, func, msg);
#else
    log::warning("%s:%d, %s\n\t%s\n", file, line, func, msg);
#endif
}
