#include <ctype.h>

char toupper(char data)
{
    for (int b = 0; b < 26; b++) {
        if (data == lower_letters[b]) {
            data = upper_letters[b];
            break;
        }
    }

    return data;
}

char tolower(char data)
{
    for (int b = 0; b < 26; b++) {
        if (data == upper_letters[b]) {
            data = lower_letters[b];
            break;
        }
    }

    return data;
}

char *toupper(char *data)
{
    for (int z = 0; z < strlen(data); z++)
        data[z] = toupper(data[z]);

    return data;
}

char *tolower(char *data)
{
    for (int z = 0; z < strlen(data); z++)
        data[z] = tolower(data[z]);

    return data;
}

int strcasecmp(const char *_l, const char *_r) {
    const uint8_t *l = (uint8_t *)_l;
    const uint8_t *r = (uint8_t *)_r;

    for (; *l && *r && (*l == *r || tolower(*l) == tolower(*r)); l++, r++);

    return tolower(*l) - tolower(*r);
}

bool strisempty(char * s)
{
    for (int z = 0; z < len(s); z++)
        if (s[z] != ' ')
            return false;

    return true;
}

bool isspace(char c)
{
    if (c == ' ')
        return true;

    return false;
}

bool isdigit(char k)
{
    if (k == '1' || k == '2' || k == '3' || k == '4' || k == '5' || k == '6' || k == '7' || k == '8' || k == '9' || k == '0')
        return true;

    return false;
}

bool isalpha(char c)
{
    c = tolower(c);

    for (int z = 0; z < 26; z++)
        if (c == lower_letters[z])
            return true;

    return false;
}
