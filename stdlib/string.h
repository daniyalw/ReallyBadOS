#pragma once
#include <list.h>
#include <stdint.h>

namespace std {

class string
{
    char * str;
    int length;
public:
    string(char * data);
    int size() { return length; };
    void append_string(char * data);
    bool operator==(char * s);
    bool operator==(string s);
    bool operator!=(char * s);
    bool operator!=(string s);
    bool operator<(string s);
    bool operator>(string s);
    bool operator>(char * s);
    bool operator<(char * s);
    void operator+(char * s);
    void operator+(string s);
    void operator=(string s);
    void operator=(char * s);
    char operator[](int z);

    char *c_str();
};

}

const char upper_letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
const char lower_letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

char * strchr(char * s, int c);
char * strip(char * s);
void clears(char * d);
uint32_t uint_atoi(char *str);
char * strcat(char * d, char * s);
char * strcpy(char * d, char * s);
char *strncpy(char *d, char *s, int n);
bool startswith(char * words, char * start);
char * append(char * orig, char * anew);
bool charcmp(char str1, char str2);
bool strcmp(char * str1, char * str2);
char * shift_left(char * out, char * str, int loc, int begin);
int len(char * str);
int strlen(char * data);
void sort(int * A, int sz);
int findMinIndex(int * A, int start, int sz);
void sort(int * A, int sz);
int findMinIndex(int * A, int start, int sz);
void swap(int num1, int num2);

void memcpy(void *dest, void * source, int begin, int end);
extern "C" void memcpy(void *dest, void *source, int length);

char * get(char * out, char *text, ...);

extern "C" void memset(void *dest, char val, u32 len);
int memcmp(void* buf1, void* buf2, int count);
void memset(u8 *dest, u8 val, u32 len);

bool find(char * s, char c);
char * strip(char * string, char key);
char * split(char * string, char key, int position, char * out);
int gvfs(char * s, char value);
bool endswith(char * words, char * end);
void itoa(unsigned int num, char * out);
void itoa(int num, char * out);
static void itoa(char *buf, int base, int d);
int atoi(char * str);
void memset(int * dest, int val, int len);

char * reverse_string(char * str, char * out);
char * reverse_string(char * str, int limit, char * out);

char strgetlast(char *c);

char *strdup(char *str);

char * strcat(char *dest, char *src);
