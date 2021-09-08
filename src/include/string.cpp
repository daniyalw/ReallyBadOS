#include "string.h"

int len(char * mychar)
{
    /* get length of `mychar` */
    int length = 0;

    while (mychar[length] != '\0')
        length++;

    return length;
}
