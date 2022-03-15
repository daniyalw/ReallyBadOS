#include <string.h>
#include <stdint.h>

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
