#include "string.h"

int len(char * words)
{
    int length = 0;

    while (words[length] != '\0')
        length++;

    return length;
}

void itoa(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';
}
void atoi(char * str, int out) {
	int length = len(str);
	int i;
	int pow = 1;
	for (i = length; i > 0; --i) {
		out += (str[i-1] - 48) * pow;
		pow *= 10;
	}
}

string::string(char * str)
{
    words = str;
    length = 0;

    while (words[length] != '\0')
        length++;
}

void string::set(char * str)
{
    words = str;
    length = 0;

    while (words[length] != '\0')
        length++;
}

int string::size()
{
    return length;
}

char * string::get()
{
    return words;
}

void string::append(char * other)
{
    for (int z = 0; z < len(other); z++)
        words += other[z];
}

int string::startswith(char * start)
{
    int clength = len(start);
    int length = len(words);

    if (length < clength)
        return 1;

    int _continue = 0;

    for (int z = 0; z < length; z++)
    {
        if (z > clength)
            break;

        if (words[z] != start[z]) // doesn't work
        {
            _continue = 1;
            break;
        }
    }

    return _continue;
}
