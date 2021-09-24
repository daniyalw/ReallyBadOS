#include "string.h"

int len(char * mychar)
{
    /* get length of `mychar` */
    int length = 0;

    while (mychar[length] != '\0')
        length++;

    return length;
}

bool startswith(char * mainchar, char * start)
{
    int clength = len(start);
    int length = len(mainchar);

    if (length < clength)
        return false;

    bool _continue = true;

    for (int z = 0; z < length; z++)
    {
        if (z > clength)
            break;

        if (mainchar[z] != start[z]) // doesn't work
        {
            _continue = false;
            break;
        }
    }

    return _continue;
}

char * first(char * word)
{
    int l;
    int done = 1;
    char * newword;

    for (int z = 0; z < len(word); z++)
    {
        if (word[z] == ' ')
        {
            if (done == 1)
            {
                l = z;
                done = 0;
                break;
            }
        }
    }

    for (int y = 0; y < l; y++)
        newword += word[y];

    return newword;
}

char * append(char * first, char * second)
{
    char * word;

    for (int z = 0; z < len(first); z++)
        word += first[z];

    for (int z = 0; z < len(second); z++)
        word += second[z];

    return word;
}

/*

string::string(char * w)
{
    words = *w;
}

int string::length()
{
    int length = 0;

    while (mychar[length] != '\0')
        length++;

    return length;
}

int string::startswith(char * start)
{
    int clength = len(start);
    int length = len(words);

    if (length < clength)
        return false;

    bool _continue = true;

    for (int z = 0; z < length; z++)
    {
        if (z > clength)
            break;

        if (mainchar[z] != start[z]) // doesn't work
        {
            _continue = false;
            break;
        }
    }

    return _continue;
}

*/
