#include <string.h>
#include <stdint.h>
#include <mem.h>

extern "C" void strcpy(char *dest, const char *orig)
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

extern "C" void memcpy(void *dest, const void *orig, int length)
{
    uint8_t *d = (uint8_t *)dest;
    uint8_t *s = (uint8_t *)orig;

    for (int z = 0; z < length; z++)
        d[z] = s[z];
}

extern "C" int strcmp(const char * str1, const char * str2)
{
    int total = strlen(str1);
    int sec = strlen(str2);

    if (total != sec) return 1;

    for (int z = 0; z < total; z++)
        if (str1[z] != str2[z]) return 1;

    return 0;
}

extern "C" char *strdup(const char *str) {
    int len = strlen(str);
    char *s = (char *)malloc(len + 1);
    memset(s, 0, len + 1);
    strcpy(s, str);
    return s;
}

namespace text {
    int len(const char *str) {
        return strlen(str);
    }

    void copy(void *d, const void *s, int length = -1) {
        if (length == -1) {
            length = text::len((const char *)s);
        }

        memcpy(d, s, length);
    }

    int compare(const char *s1, const char *s2) {
        return strcmp(s1, s2);
    }

    char *duplicate(const char *s) {
        return strdup(s);
    }

    void set(void *dest, char val, int length) {
        memset(dest, val, length);
    }
}
