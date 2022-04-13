#pragma once

#include <itoa.h>
#include <atoi.h>
#include "_header.h"

const char upper_letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
const char lower_letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

C_HEADER

extern int strlen(char * text);
extern void strcpy(char *dest, char *orig);
extern void memset(void *dest, char val, int length);
extern void memcpy(void *dest, void *orig, int length);
extern int strcmp(char * str1, char * str2);
char *strdup(char *str);

C_HEADER_END
