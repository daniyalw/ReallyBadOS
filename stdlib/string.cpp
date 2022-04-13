#pragma once
#include <string.h>
#include <ctype.h>
#include <list.h>
#include <memory.h>
#include <strtol.h>

int atoi(char * str) {
    int num = 0;

    for (int z = 0; str[z] != '\0'; z++)
        num = num * 10 + str[z] - '0';

    return num;
}

uint32_t uint_atoi(char *str) {
    uint32_t num = 0;

    for (int z = 0; str[z] != '\0'; z++)
        num = num * 10 + str[z] - '0';

    return num;
}

void memcpy(void *dest, void * source, int begin, int end) {
    for (int z = begin; z < end; z++)
        ((int *)dest)[z] = ((int *)source)[z];
}

extern "C" void memcpy(void *dest, void *source, int length) {
    memcpy(dest, source, 0, length);
}

bool contains(char *str, char c) {
    for (int z = 0; z < strlen(str); z++)
        if (str[z] == c)
            return true;

    return false;
}

int tokenize(char *s, char sep, char **buff) {
    int bufsize = 100;
    int off = 0;
    int bz = 0;

    if (s[strlen(s) - 1] != sep) {
        s[strlen(s)] = sep;
    }

    char *buf = (char *)malloc(bufsize);
    memset(buf, 0, bufsize);

    for (int z = 0; z < strlen(s); z++) {
        if (s[z] == sep && buf) {
            buff[off] = buf;

            // reset
            bufsize = 100;
            buf = (char *)malloc(bufsize);
            memset(buf, 0, bufsize);

            off++;
            bz = 0;

            continue;
        } else if (s[z] == sep) {
            continue;
        }

        buf[bz] = s[z];
        bz++;
    }

    return off;
}

char strgetlast(char *c) {
    return c[strlen(c)-1];
}

static void itoa(char *buf, int base, int d) {
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = d;
  int divisor = 10;

  if (base == 'd' && d < 0)
    {
      *p++ = '-';
      buf++;
      ud = -d;
    }
  else if (base == 'x')
    divisor = 16;

  do
    {
      int remainder = ud % divisor;

      *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
  while (ud /= divisor);

  *p = 0;

  p1 = buf;
  p2 = p - 1;
  while (p1 < p2)
    {
      char tmp = *p1;
      *p1 = *p2;
      *p2 = tmp;
      p1++;
      p2--;
    }
}

void itoa(int num, char * out) {
    itoa(out, 10, num);
}

void itoa(uint32_t num, char * out) {
    itoa(out, 10, num);
}

int len(char * str) {
    int count = 0;

    while (str[count] != '\0')
        count++;

    return count;
}

bool strcmp(char * str1, char * str2) {
    int total = strlen(str1);
    int sec = strlen(str2);

    if (total != sec) return true;

    for (int z = 0; z < total; z++)
        if (str1[z] != str2[z]) return true;

    return false;
}

char * shift_left(char * out, char * str, int loc, int begin) {
    int length = strlen(str);
    char s[length];
    int sz = 0;

    for (int z = 0; z < length; z++)
        s[z] = 0;

    for (int z = begin; z < length; z++) {
        s[sz] = str[z];
        sz++;
    }

    out = s;

    return out;
}

bool charcmp(char str1, char str2) {
    if (str1 == str2)
        return true;

    return false;
}

char *append(char *a1, char *a2, char *out) {
    int c = 0;

    for (int z = 0; z < strlen(a1); z++) {
        out[c] = a1[z];
        c++;
    }

    for (int z = 0; z < strlen(a2); z++) {
        out[c] = a2[z];
        c++;
    }

    out[c] = NULL;

    return out;
}

char * strcat(char *dest, char *src) {
    int c = strlen(dest);

    for (int z = 0; z < strlen(src); z++) {
        dest[c] = src[z];
        c++;
    }

    dest[c] = 0;

    return dest;
}

char * append_int(char * j1, int num, char * dest) {
    char * textnum;
    itoa(num, textnum);
    strcat(dest, j1);
    strcat(dest, textnum);
    return dest;
}

int vsscanf(char *str, char *fmt, va_list va) {
	int cz = 0;

	while (*fmt) {
		if (*fmt == ' ') {
			while (*str && isspace(*str))
				str++;
		} else if (*fmt == '%') {
			fmt++;

			if (*fmt == 'd') {
				int z = 0;
				int negative = 1;

				while (isspace(*str)) str++;

                // see if the first is a minus sign
				if (*str == '-') {
					negative = -1;
					str++;
				}

				while (*str && *str >= '0' && *str <= '9') {
					z = z * 10 + *str - '0';
					str++;
				}

				int * out = (int *)va_arg(va, int*);
				cz++;
				*out = z * negative;
			} else if (*fmt == 'u') {
				uint32_t z = 0;

				while (isspace(*str)) str++;

				while (*str && *str >= '0' && *str <= '9') {
					z = z * 10 + *str - '0';
					str++;
				}

				uint32_t *out = (uint32_t *)va_arg(va, uint32_t*);
				cz++;
				*out = z;
			} else if (*fmt == 'x') {
                uint32_t z = 0;

                while (isspace(*str)) str++;

                z = strtol(str, NULL, 16);
                int *out = (int *)va_arg(va, int*);
                cz++;
                *out = z;
            } else if (*fmt == 's') {
                char **out = (char **)va_arg(va, char**);
                char *s = (char *)&out[0];
                int sz = 0;

                while (isspace(*str)) str++;

                while (isalpha(*str)) {
                    s[sz] = *str;
                    *str++;
                    sz++;
                }

                s[sz] = 0;
                cz++;
            }
		} else {
			if (*str == *fmt)
				str++;
			else
				break;
		}

		fmt++;
	}

	return cz;
}

int sscanf(char *s, char *fmt, ...) {
	va_list va;
    int out;

	va_start(va, fmt);
	out = vsscanf(s, fmt, va);
	va_end(va);

	return out;
}

char *strstr(char *s, char *d) {
	for (int i = 0; s[i]; i++) {
		bool found = false;

		for (int j = 0; d[j]; j++) {
			if (!d[j] || s[i + j] == d[j])
				continue;

			found = true;
			break;
		}

		if (!found)
			return (char *)&s[i];
	}

	return NULL;
}

char *strtok_r(char *s, char *del, char **m) {
	char *tok;

	if (s)
		tok = s;
	else if (*m)
		tok = *m;
	else
		return NULL;

	while (*tok && strchr(del, *tok))
		tok++;

	auto p = tok;

	while (*p && !strchr(del, *p))
		p++;

	if (*p) {
		*p = 0;
		*m = p + 1;
	} else {
		*m = NULL;
	}

	if (p == tok)
		return NULL;

	return tok;
}

char *strtok(char *s, char *del) {
        static char *saved;
        return strtok_r(s, del, &saved);
}

bool startswith(char * words, char * start) {
    int cl = len(start);
    int length = len(words);

    if (cl > length) {
        return false;
    }

    for (int z = 0; z < cl; z++) {
        if (words[z] != start[z]) {
            return false;
        }
    }

    return true;
}

bool endswith(char * words, char * end) {
    int x = 0;

    for (int z = strlen(words) - strlen(end); z < strlen(words); z++) {
        if (words[z] != end[x])
            return false;

        x++;
    }

    return true;
}

char * strcpy(char * d, char * s) {
    for (int z = 0; z < strlen(s); z++) {
        d[z] = s[z];
    }

    d[strlen(s)] = 0;

    return d;
}

char *strncpy(char *d, char *s, int n) {
    const int max = strlen(s);
    int c = 0;

    for (int z = 0; z < max; z++) {
        if (z == n)
            break;

        d[z] = s[z];
        c++;
    }

    d[c] = 0;

    return d;
}

// get how many times a value appears in a string
int gvfs(char * s, char value) {
    int count = 0;

    for (int z = 0; z < len(s); z++)
        if (s[z] == value)
            count++;

    return count;
}

char * reverse_string(char * str, int limit, char * out) {
    int v = 0;

    for (int z = strlen(str) - 1; z >= 0; z--) {
        if (v >= limit)
            break;

        out[v] = str[z];
        v++;
    }

    return out;
}

char *reverse_string(char *str, char *out) {
    int v = 0;

    for (int z = strlen(str) - 1; z >= 0; z--) {
        out[v] = str[z];
        v++;
    }

    out[v] = 0;

    return out;
}

char * opposite_split(char * string, char key, int position, char * out) {
    int lcount = 0;
    int v = 0;

    for (int z = len(string); z > 0; z--) {
        if (string[z] == key) {
            if (lcount == position) {
                out = reverse_string(string, out);
                return out;
            }

            lcount++;
            out = "";
            v = 0;
        }
        else {
            out[v] = string[z];
            v++;
            out[v] = 0;
        }
    }

    return out;
}

// string: the variable to split
// key: the delimeter of the split
// position: which part of the split string to return
// out: where to store the return value
char * split(char * string, char key, int position, char * out) {
    int lcount = 0;
    int v = 0;

    for (int z = 0; z < len(string); z++) {
        if (string[z] == key) {
            if (lcount == position)
                return out;

            lcount++;
            out = "";
            v = 0;
        } else {
            out[v] = string[z];
            v++;
            out[v] = 0;
        }
    }

    return out;
}

char * get(char * out, char *text, ...) {
    int max = strlen(text);
    va_list va;

    va_start(va, text);
    vsnprintf(out, max, text, va);
    va_end(va);

    return out;
}

char * strip(char * string, char key) {
    int sz = 0;
    char buff[len(string)];

    for (int z = 0; z < len(string); z++) {
        if (string[z] != key) {
            buff[sz] = string[z];
            sz++;
        }
    }

    buff[sz] = 0;

    string = buff;

    return string;
}

char * strchr(char * s, int c) {
    while (*s != (char) c) {
        if (!*s++) {
            return NULL;
        }
    }
    return s;
}

bool find(char * s, char c) {
    for (int z = 0; z < len(s); z++) {
        if (s[z] == c)
            return true;
    }

    return false;
}

void memset(u8 *dest, u8 val, u32 len) {
    u8 *temp = (u8 *)dest;

    for (; len != 0; len--)
    {
        *temp++ = val;
    }
}

void memset(int * dest, int val, int len) {
    int *temp = (int *)dest;

    for (int z = len; z != 0; z--) {
        *temp++ = val;
    }
}

extern "C" void memset(void *dest, char val, u32 len) {
    char *temp = (char *)dest;

    for (; len != 0; len--) {
        *temp++ = val;
    }
}

int memcmp(void* buf1, void* buf2, int count) {
    if(!count)
        return(0);

    while(--count && *(char*)buf1 == *(char*)buf2 ) {
        buf1 = (char*)buf1 + 1;
        buf2 = (char*)buf2 + 1;
    }

    return(*((unsigned char*)buf1) - *((unsigned char*)buf2));
}

int strlen(char * data) {
    int z = 0;

    while (data[z])
        z++;

    return z;
}

void swap(int num1, int num2) {
    int num3 = num1;
    num1 = num2;
    num2 = num3;
}

namespace std {

string::string(char * data) {
    length = strlen(data);
    str = (char *)malloc(length + 1);

    if (!str)
        return;

    memset(str, 0, length + 1);
    strcpy(str, data);
}

void string::append_string(char * data) {
    strcat(str, data);
    length += strlen(data);
}

void string::operator=(char * s) {
    length = strlen(s);
    str = (char *)realloc(str, length + 1);

    if (!str)
        return;

    memset(str, 0, length + 1);
    strcpy(str, s);
}

void string::operator=(string s) {
    length = s.size();
    str = (char *)realloc(str, length + 1);

    if (!str)
        return;

    memset(str, 0, length + 1);
    strcpy(str, s.c_str());
}

bool string::operator==(string s) {
    if (strcmp(s.c_str(), this->c_str()) == 0 && s.size() == this->length)
        return true;

    return false;
}

bool string::operator==(char * s) {
    if (strcmp(s, this->c_str()) == 0 && strlen(s) == this->length)
        return true;

    return false;
}

bool string::operator!=(string s) {
    if (strcmp(s.c_str(), this->c_str()) && s.size() == this->length)
        return false;

    return true;
}

bool string::operator!=(char * s) {
    if (strcmp(s, this->c_str()) && len(s) == this->length)
        return false;

    return true;
}

bool string::operator<(string s) {
    if (this->size() < s.size())
        return true;

    return false;
}

bool string::operator<(char * s) {
    if (this->length < len(s))
        return true;

    return false;
}

bool string::operator>(string s) {
    if (this->size() > s.size())
        return true;

    return false;
}

bool string::operator>(char * s) {
    if (this->length > strlen(s))
        return true;

    return false;
}

void string::operator+(string s) {
    strcat(this->str, s.c_str());
}

void string::operator+(char * s) {
    strcat(this->str, s);
}

char string::operator[](int z) {
    return str[z];
}

char * string::c_str() {
    return str;
}

}

void memset(char * dest, char *val, int len) {
    for (int z = 0; z < len; z++)
    {
        dest[z] = val[z];
    }
}

char *strdup(char *str) {
    const int length = strlen(str);
    char *text = (char *)malloc(length + 1);
    memset(text, 0, length + 1);
    strcpy(text, str);
    return text;
}

char *strjoin(char **buf, char *del, char *out) {
    int size = 0;
    int count = 0;
    const int del_length = strlen(del);

    while (buf[count]) {
        count++;
    }

    for (int z = 0; z < count; z++) {
        size += strlen(buf[z]);
        size += del_length;
    }

    int c = 0;

    for (int z = 0; z < count; z++) {
        for (int b = 0; b < strlen(buf[z]); b++) {
            out[c] = buf[z][b];
            c++;
        }

        if (z == count - 1) {
            break;
        }

        for (int b = 0; b < del_length; b++) {
            out[c] = del[b];
            c++;
        }
    }

    c -= del_length * 4;
    out[c] = NULL;

    return out;
}
