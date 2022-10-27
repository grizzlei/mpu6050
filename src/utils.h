#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif


////////////////////////////////////////////


#define prdbg(x, ...) (printf("\x1b[33mdebug:: " x "\x1b[0m\n", ##__VA_ARGS__))
#define prerr(x, ...) (printf("\x1b[31merror:: " x "\x1b[0m\n", ##__VA_ARGS__))
#define prwar(x, ...) (printf("\x1b[36mwarng:: " x "\x1b[0m\n", ##__VA_ARGS__))
#define UNUSED(x) ((void)x);


////////////////////////////////////////////


// wraps system calls to show a message on error (when res is -1).
int sysguard(int res, char *msg);


////////////////////////////////////////////


// get time in seconds up to nano seconds
double get_time_sec();


////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif