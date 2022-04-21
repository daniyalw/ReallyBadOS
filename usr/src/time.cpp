#include <syscalls.h>

typedef struct {
    int sec, min, h, d, m, y, wd;
    bool pm;
} time_t;

extern "C" time_t get_time()
{
    time_t time;
    void *a;
    CALL_SYS1(a, TIME, &time);
    return time;
}

namespace Time {
    time_t time() {
        return get_time();
    }
}
