#include <string.h>

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
