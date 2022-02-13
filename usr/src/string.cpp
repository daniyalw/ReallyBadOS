#include <string.h>

extern "C" void strcpy(char *dest, char *orig)
{
    for (int z = 0; z < strlen(orig); z++)
        dest[z] = orig[z];
}
