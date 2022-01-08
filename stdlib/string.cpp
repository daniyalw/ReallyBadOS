#pragma once
#include <string.h>
#include <list.h>

namespace std {

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

int atoi(char * str)
{
    int num = 0;

    for (int z = 0; str[z] != '\0'; z++)
        num = num * 10 + str[z] - '0';

    return num;
}

void memcpy(char *source, char * dest, int begin, int end)
{
    //ASSERT(begin < end);

    for (int z = begin; z < end; z++)
    {
        dest[z] = source[z];
    }
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

void itoa(unsigned int num, char * out)
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

    if (total != sec) return false;

    for (int z = 0; z < total; z++)
        if (str1[z] != str2[z]) return false;

    return true;
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

void memset(char * dest, char *val, int len)
{
    for (int z = 0; z < len; z++)
    {
        dest[z] = val[z];
    }
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

void memcpy(char *source, char *dest, int nb)
{
    for (int z = 0; z < nb; z++)
        dest[z] = source[z];
}

void memcpy_int(int *source, int *dest, int nb)
{
    for (int z = 0; z < nb; z++)
        dest[z] = source[z];
}

char * get(char * out, char *text, ...)
{
  char **arg = (char **) &text;
  int c;
  char buffer[20];
  int st = 0;

  arg++;

  while ((c = *text++) != 0)
    {
      if (c != '%')
      {
          out[st] = c;
          st++;
      }
      else
        {
          char *p, *p2;
          int pad0 = 0, pad = 0;

          c = *text++;
          if (c == '0')
            {
              pad0 = 1;
              c = *text++;
            }

          if (c >= '0' && c <= '9')
            {
              pad = c - '0';
              c = *text++;
            }

          switch (c)
            {
            case 'c':
                out[st] = c;
                st++;
                break;
            case 'd':
            case 'u':
            case 'x':
              std::itoa (buffer, c, *((int *) arg++));
              p = buffer;
              goto string;
              break;

            case 's':
              p = *arg++;
              if (! p)
                p = "(null)";

            string:
              for (p2 = p; *p2; p2++);
              for (; p2 < p + pad; p2++)
              {
                out[st] = pad0 ? '0' : ' ';
                st++;
              }
              while (*p)
              {
                out[st] = *p++;
                st++;
              }
              break;

            default:
              out[st] = *((int *) arg++);
              st++;
              break;
            }
        }
    }

    out[st] = 0;

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

bool strisempty(char * s)
{
    for (int z = 0; z < len(s); z++)
        if (s[z] != ' ')
            return false;

    return true;
}

char * toupper(char * data)
{
    char upper_letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    char lower_letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    for (int z = 0; z < len(data); z++) {
        for (int b = 0; z < 26; b++) {
            if (data[z] == lower_letters[b]) {
                data[z] = upper_letters[b];
                break;
            }
        }
    }

    return data;
}

char toupper(char data)
{
    char upper_letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    char lower_letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    for (int b = 0; b < 26; b++) {
        if (data == lower_letters[b]) {
            data = upper_letters[b];
            break;
        }
    }

    return data;
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

void memset(char *dest, char val, u32 len) {
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

int findMinIndex(std::list<int> A, int start) {
    int min_index = start;

    ++start;

    while(start < (int)A.size()) {
        if(A.get(start) < A.get(min_index))
            min_index = start;

        ++start;
    }

    return min_index;
}

void sort(std::list<int> A) {
    for(int i = 0; i < (int)A.size(); ++i) {
        int min_index = findMinIndex(A, i);

        if(i != min_index)
            A.swap(i, min_index);
    }
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

string::string(char * data)
{
    str = data;
    length = std::len(data);
}

void string::append_string(char * data)
{
    append(str, data, str);
    length += len(data);
}

void string::operator=(char * s)
{
    str = s;
    length = std::len(s);
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
    if (s == this->get() && std::len(s) == this->length)
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
    if (s == this->get() && std::len(s) == this->length)
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
    if (this->length < std::len(s))
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
    if (this->length > std::len(s))
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
