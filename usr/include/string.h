#pragma once

#include <itoa.h>
#include <atoi.h>
#include "_header.h"

const char upper_letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
const char lower_letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

C_HEADER

extern int strlen(const char * text);
extern void strcpy(char *dest, const char *orig);
extern void memset(void *dest, char val, int length);
extern void memcpy(void *dest, const void *orig, int length);
extern int strcmp(const char * str1, const char * str2);
extern char *strdup(const char *str);
extern int strtol(char *nptr, char **endptr, int base);
extern char * strchr(char * s, int c);
extern int vsscanf(char *str, char *fmt, va_list va);
extern int sscanf(char *s, char *fmt, ...);
extern char *strstr(char *s, char *d);
extern char *strtok_r(char *s, char *del, char **m);
extern char *strtok(char *s, char *del);

C_HEADER_END

#ifdef __cplusplus
namespace text {

extern int len(const char *str);
extern void copy(void *d, const void *s, int length = -1);
extern int compare(const char *s1, const char *s2);
extern char *duplicate(const char *s);
extern void set(void *dest, char val, int length);

}
#endif
