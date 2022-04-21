#pragma once

#include <stdarg.h>
#include "_header.h"

C_HEADER

extern int printf(char *a, ...);
extern void putchar(char text);
extern void snprintf(char *s, int max, char *fmt, ...);
extern void sprintf(char *s, char *fmt, ...);
extern int vprintf(char *f, va_list va);
extern char * vsprintf(char *s, char *format, va_list va);
extern char * vnprintf(char *s, int max, char *format, va_list va);

C_HEADER_END

#ifdef __cplusplus
namespace text {

extern int write(char *fmt, ...);

}

#endif
