#pragma once
#include <list.h>
#include <stdint.h>

namespace std {

class string
{
    char * str;
    int length;
public:
    char * get();
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

char * strchr(char * s, int c);
char * toupper(char * data);
bool strisempty(char * s);
char * strip(char * s);
void clears(char * d);
char * strcat(char * d, char * s);
char * strcpy(char * d, char * s);
bool startswith(char * words, char * start);
char * append(char * orig, char * anew);
bool charcmp(char str1, char str2);
bool strcmp(char * str1, char * str2);
char * shift_left(char * out, char * str, int loc, int begin);
int len(char * str);
int strlen(char * data);
void sort(int * A, int sz);
int findMinIndex(int * A, int start, int sz);
void sort(std::list<int> A);
void sort(int * A, int sz);
int findMinIndex(int * A, int start, int sz);
void sort(std::list<int> A);
int findMinIndex(std::list<int> A, int start);
void swap(int num1, int num2);
inline void memcpy(const char *source, char *dest, const int nb);
void memcpy(char *source, char * dest, int begin, int end);
char * get(char * out, char *text, ...);
inline void memcpy_int(const int *source, int *dest, const int nb);
void memset(char *dest, char val, u32 len);
void memset(char * dest, char *val, int len);
int memcmp(void* buf1, void* buf2, int count);
void memset(u8 *dest, u8 val, u32 len);
bool find(char * s, char c);
char * strip(char * string, char key);
char * split(char * string, char key, int position, char * out);
int gvfs(char * s, char value);
bool endswith(char * words, char * end);
char * append(char * j1, char * j2, char * dest);
void itoa(unsigned int num, char * out);
void itoa(int num, char * out);
static void itoa(char *buf, int base, int d);
int atoi(char * str);
bool isspace(char c);
bool isdigit(char k);
void memset(int * dest, int val, int len);
char * reverse_string(char * str, char * out);

}
