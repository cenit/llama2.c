#include "win.h"

// Portable clock_gettime function for Windows
int clock_gettime(int clk_id, struct timespec *tp) {
    uint32_t ticks = GetTickCount();
    tp->tv_sec = ticks / 1000;
    tp->tv_nsec = (ticks % 1000) * 1000000;
    return 0;
}
