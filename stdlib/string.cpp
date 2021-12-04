#pragma once
#include <string.h>
#include <list.h>

int get_sizeof_var(void * data)
{
    void * newdata;
    newdata = data;
    return sizeof(newdata);
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

int atoi(char * str)
{
    int num = 0;

    for (int z = 0; str[z] != '\0'; z++)
        num = num * 10 + str[z] - '0';

    return num;
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
    int total_length = len(str1);
    int slength = len(str2);

    if (total_length < slength || total_length > slength) {
        return false;
    }

    for (int z = 0; z < total_length; z++) {
        if (str1[z] != str2[z]) {
            return false;
        }
    }

    return true;
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
            {
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

void memcpy(u8 *source, u8 *dest, int nb) {
    int i;
    for (i = 0; i < nb; i++) {
        *(dest + i) = *(source + i);
    }
}

void memcpy(char *source, char *dest, int nb) {
    int i;
    for (i = 0; i < nb; i++) {
        *(dest + i) = *(source + i);
    }
}

void memset(u8 *dest, u8 val, u32 len) {
    u8 *temp = (u8 *)dest;

    for (; len != 0; len--)
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
