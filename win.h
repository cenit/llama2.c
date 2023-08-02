#ifndef _WIN_H_
#define _WIN_H_

#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <time.h>

#define ssize_t __int64
#define CLOCK_REALTIME 0

#ifdef __cplusplus
extern "C" {
#endif

int     clock_gettime(int clk_id, struct timespec *tp);

#ifdef __cplusplus
};
#endif

#endif /*  _WIN_H_ */
