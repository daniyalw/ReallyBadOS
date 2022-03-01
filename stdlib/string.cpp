#pragma once
#include <string.h>
#include <ctype.h>
#include <list.h>

int atoi(char * str)
{
    int num = 0;

    for (int z = 0; str[z] != '\0'; z++)
        num = num * 10 + str[z] - '0';

    return num;
}

uint32_t uint_atoi(char *str)
{
    uint32_t num = 0;

    for (int z = 0; str[z] != '\0'; z++)
        num = num * 10 + str[z] - '0';

    return num;
}

void memcpy(void *dest, void * source, int begin, int end)
{
    //ASSERT(begin < end);

    for (int z = begin; z < end; z++)
    {
        ((short *)dest)[z] = ((short *)source)[z];
    }
}

extern "C" void memcpy(void *dest, void *source, int length)
{
    memcpy(dest, source, 0, length);
}

bool contains(char *str, char c)
{
    for (int z = 0; z < strlen(str); z++)
        if (str[z] == c)
            return true;

    return false;
}

auto tokenize(char *s, char sep, char **buff, int beg_index, int end_index)
{
    const int total_length = strlen(s);
    int length = 0;
    int count = 1;
    char *str = (char *)malloc(total_length);
    uint32_t orig_addr = (uint32_t)&str;

    str[total_length] = sep;
    str[total_length] = NULL;
    strcpy(str, s);

    for (int z = 0; z < total_length; z++)
    {
        if (str[z] == sep)
        {
            char *ss = (char *)malloc(strlen(str));
            strcpy(ss, str);
            ss[length] = 0;
            str = (char *)&str[length + 1];
            length = 0;

            buff[count-1] = ss;

            free(ss);

            count++;
        }
        else
        {
            length++;
        }
    }

    char *ss = (char *)malloc(strlen(str));
    strcpy(ss, str);

    ss = (char *)&ss[beg_index];
    ss[strlen(ss)-end_index] = 0;

    buff[count-1] = ss;

    free(ss);

    return count;
}

int tokenize(char *str, char sep, char **buff)
{
    return tokenize(str, sep, buff, 0, 0);
}

char strgetlast(char *c)
{
    return c[strlen(c)-1];
}

static void itoa(char *buf, int base, int d)
{
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

void itoa(int num, char * out)
{
    itoa(out, 10, num);
}

void itoa(uint32_t num, char * out)
{
    itoa(out, 10, num);
}

int len(char * str)
{
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

// ex:
//      char * a = "/";
//      char * b = "abc";
//      char * dest;
//      append(a, b, dest);
//
// dest will be "/abc"
char * append(char * j1, char * j2, char * dest)
{
    int js = 0;

    for (int z = 0; z < len(j1); z++)
    {
        dest[js] = j1[z];
        js++;
    }

    for (int z = 0; z < len(j2); z++)
    {
        dest[js] = j2[z];
        js++;
    }

    dest[js] = 0;

    return dest;
}

char * append_int(char * j1, int num, char * dest)
{
    char * textnum;
    itoa(num, textnum);
    return append(j1, textnum, dest);
}

int vsscanf(char *str, char *fmt, va_list va)
{
	int cz = 0;

	while (*fmt)
	{
		if (*fmt == ' ')
		{
			while (*str && isspace(*str))
				str++;
		}
		else if (*fmt == '%')
		{
			fmt++;

			if (*fmt == 'd')
			{
				int z = 0;
				int negative = 1;

				while (isspace(*str)) str++;

                // see if the first is a minus sign
				if (*str == '-')
				{
					negative = -1;
					str++;
				}

				while (*str && *str >= '0' && *str <= '9')
				{
					z = z * 10 + *str - '0';
					str++;
				}

				int * out = (int *)va_arg(va, int*);
				cz++;
				*out = z * negative;
			}
			else if (*fmt == 'u')
			{
				uint32_t z = 0;

				while (isspace(*str)) str++;

				while (*str && *str >= '0' && *str <= '9')
				{
					z = z * 10 + *str - '0';
					str++;
				}

				uint32_t *out = (uint32_t *)va_arg(va, uint32_t*);
				cz++;
				*out = z;
			}
            else if (*fmt == 's')
            {
                char **out = (char **)va_arg(va, char**);
                char *s = (char *)&out[0];
                int sz = 0;

                while (isspace(*str)) str++;

                while (isalpha(*str))
                {
                    s[sz] = *str;
                    *str++;
                    sz++;
                }

                s[sz] = 0;
                cz++;
            }
		}
		else
		{
			if (*str == *fmt)
				str++;
			else
				break;
		}

		fmt++;
	}

	return cz;
}

int sscanf(char *s, char *fmt, ...)
{
	va_list va;
    int out;

	va_start(va, fmt);
	out = vsscanf(s, fmt, va);
	va_end(va);

	return out;
}

char *strstr(char *s, char *d)
{
	for (int i = 0; s[i]; i++)
    {
		bool found = false;

		for (int j = 0; d[j]; j++)
        {
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

char *strtok_r(char *s, char *del, char **m)
{
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

	if (*p)
    {
		*p = 0;
		*m = p + 1;
	}
    else
    {
		*m = NULL;
	}

	if (p == tok)
		return NULL;

	return tok;
}

char *strtok(char *s, char *del)
{
        static char *saved;
        return strtok_r(s, del, &saved);
}

int tokenize(char * str, char * d, char **buf)
{
	char *p;
	char *s;
	int args = 0;

	p = strtok_r(str, d, &s);

	if (!p)
        return 0;

	while (p != NULL)
    {
		buf[args] = (char *)p;
		args++;
		p = strtok_r(NULL, d, &s);
	}

	buf[args] = NULL;
	return args;
}

bool startswith(char * words, char * start)
{
    int cl = len(start);
    int length = len(words);

    if (cl > length)
    {
        return false;
    }

    for (int z = 0; z < cl; z++)
    {
        if (words[z] != start[z])
        {
            return false;
        }
    }

    return true;
}

bool endswith(char * words, char * end)
{
    int x = 0;

    for (int z = len(words)-len(end); z < len(words); z++)
    {
        if (words[z] != end[x])
            return false;

        x++;
    }

    return true;
}

char * strcpy(char * d, char * s) // destination, source
{
    while ((*d++ = *s++));
    return d;
}

char * strcat(char * d, char * s)
{
    strcpy(d + len(d), s);
    return d;
}

void clears(char d[128])
{
    for (int z = 0; z < 128; z++)
        d[z] = ' ';
}

// get how many times a value appears in a string
int gvfs(char * s, char value)
{
    int count = 0;

    for (int z = 0; z < len(s); z++)
        if (s[z] == value)
            count++;

    return count;
}

char * reverse_string(char * str, char * out)
{
    int t = strlen(str);
    int c = t;

    for (int z = 0; z < strlen(str); z++) {
        out[t] = str[z];
        t--;
    }

    char a[c-1];

    for (int z = 0; z < c; z++)
    {
        a[z] = out[z+1];
    }

    out = a;

    return out;
}

char * opposite_split(char * string, char key, int position, char * out)
{
    int lcount = 0;
    int v = 0;

    for (int z = len(string); z > 0; z--)
    {
        if (string[z] == key)
        {
            if (lcount == position)
            {
                out = reverse_string(string, out);
                return out;
            }
            lcount++;
            out = "";
            v = 0;
        }
        else
        {
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
char * split(char * string, char key, int position, char * out)
{
    int lcount = 0;
    int v = 0;

    for (int z = 0; z < len(string); z++)
    {
        if (string[z] == key)
        {
            if (lcount == position)
                return out;

            lcount++;
            out = "";
            v = 0;
        }
        else
        {
            out[v] = string[z];
            v++;
            out[v] = 0;
        }
    }

    return out;
}

#if defined(__x86_64__) || defined(__x86_32__)
static inline void __movsb_copy_str(char *from, const char *to, int size) {
  asm volatile ("rep movsb"
                : "=D" (from),
                  "=S" (to),
                  "=c" (size)
                : "D" (from),
                  "S" (to),
                  "c" (size)
                : "memory");

}

#endif

#if defined(__x86_64__) || defined(__x86_32__)
static inline void __movsb_copy_int(int *to, const int *from, int size) {
  asm volatile ("rep movsb"
                : "=D" (from),
                  "=S" (to),
                  "=c" (size)
                : "D" (from),
                  "S" (to),
                  "c" (size)
                : "memory");

}

#endif

inline void memcpy(char *dest, const char *source, const int nb)
{
    #if defined(__x86_64__) || defined(__x86_32__)

    __movsb_copy_str(source, dest, nb);

    #else

    for (int z = 0; z < nb; z++)
        dest[z] = source[z];

    #endif
}

inline void memcpy_int(const int *source, int *dest, const int nb)
{
    #if defined(__x86_64__) || defined(__x86_32__)

    __movsb_copy_int(source, dest, nb);

    #else

    for (int z = 0; z < nb; z++)
        dest[z] = source[z];

    #endif
}

char * get(char * out, char *text, ...)
{
    int max = strlen(text);
    va_list va;

    va_start(va, text);
    vsnprintf(out, max, text, va);
    va_end(va);

    return out;
}

char * strip(char * string, char key)
{
    int sz = 0;
    char buff[len(string)];

    for (int z = 0; z < len(string); z++)
    {
        if (string[z] != key)
        {
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

    for (int z = len; z != 0; z--)
    {
        *temp++ = val;
    }
}

extern "C" void memset(char *dest, char val, u32 len) {
    char *temp = (char *)dest;

    for (; len != 0; len--)
    {
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

int strlen(char * data)
{
    return len(data);
}

void swap(int num1, int num2)
{
    int num3 = num1;
    num1 = num2;
    num2 = num3;
}

int findMinIndex(int * A, int start, int sz) {
    int min_index = start;

    ++start;

    while(start < sz) {
        if(A[start] < A[min_index])
            min_index = start;

        ++start;
    }

    return min_index;
}

void sort(int * A, int sz) {
    for(int i = 0; i < sz; ++i) {
        int min_index = findMinIndex(A, i, sz);

        if(i != min_index)
        {
            int temp;
            temp = A[i];
            A[i] = A[min_index];
            A[min_index] = temp;
        }
    }
}

namespace std {

string::string(char * data)
{
    str = data;
    length = len(data);
}

void string::append_string(char * data)
{
    append(str, data, str);
    length += len(data);
}

void string::operator=(char * s)
{
    str = s;
    length = len(s);
}

void string::operator=(string s)
{
    str = s.get();
    length = s.size();
}

char * string::get()
{
    return str;
}

bool string::operator==(string s)
{
    if (s.get() == this->get() && s.size() == this->length)
        return true;

    return false;
}

bool string::operator==(char * s)
{
    if (s == this->get() && len(s) == this->length)
        return true;

    return false;
}

bool string::operator!=(string s)
{
    if (s.get() == this->get() && s.size() == this->length)
        return false;

    return true;
}

bool string::operator!=(char * s)
{
    if (s == this->get() && len(s) == this->length)
        return false;

    return true;
}

bool string::operator<(string s)
{
    if (this->size() < s.size())
        return true;

    return false;
}

bool string::operator<(char * s)
{
    if (this->length < len(s))
        return true;

    return false;
}

bool string::operator>(string s)
{
    if (this->size() > s.size())
        return true;

    return false;
}

bool string::operator>(char * s)
{
    if (this->length > len(s))
        return true;

    return false;
}

void string::operator+(string s)
{
    append(this->get(), s.get(), this->str);
}

void string::operator+(char * s)
{
    append(this->str, s, this->str);
}

char string::operator[](int z)
{
    return str[z];
}

char * string::c_str()
{
    return str;
}

}

void memset(char * dest, char *val, int len)
{
    for (int z = 0; z < len; z++)
    {
        dest[z] = val[z];
    }
}
