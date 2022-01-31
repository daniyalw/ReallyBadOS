#include <syscalls.h>

typedef struct {
    int sec, min, h, d, m, y, wd;
    bool pm;
} time_t;

extern "C" time_t get_time()
{
    time_t time[1];
    void * a;
    asm volatile("int $48" : "=a" (a) : "0" (TIME), "b" (time));
    return time[0];
}
