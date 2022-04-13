#include <string.h>
#include <stdint.h>
#include <stdlib.h>

extern "C" void strcpy(char *dest, char *orig)
{
    for (int z = 0; z < strlen(orig); z++)
        dest[z] = orig[z];
}

extern "C" void memset(void *dest, char val, int length)
{
    unsigned char *d = (unsigned char *)dest;

    for (int z = 0; z < length; z++)
        d[z] = val;
}

extern "C" void memcpy(void *dest, void *orig, int length)
{
    uint8_t *d = (uint8_t *)dest;
    uint8_t *s = (uint8_t *)orig;

    for (int z = 0; z < length; z++)
        d[z] = s[z];
}

extern "C" int strcmp(char * str1, char * str2)
{
    int total = strlen(str1);
    int sec = strlen(str2);

    if (total != sec) return 1;

    for (int z = 0; z < total; z++)
        if (str1[z] != str2[z]) return 1;

    return 0;
}

extern "C" char *strdup(char *str) {
    int len = strlen(str);
    char *s = (char *)malloc(len + 1);
    memset(s, 0, len + 1);
    strcpy(s, str);
    return s;
}
