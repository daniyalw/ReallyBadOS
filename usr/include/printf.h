#pragma once

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void printf(char *a, ...);
extern void putchar(char text);
extern void snprintf(char *s, int max, char *fmt, ...);
extern void sprintf(char *s, char *fmt, ...);
extern void vprintf(char *f, va_list va);
extern char * vsprintf(char *s, char *format, va_list va);
extern char * vnprintf(char *s, int max, char *format, va_list va);

#ifdef __cplusplus
}
#endif
