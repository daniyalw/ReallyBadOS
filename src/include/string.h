#pragma once

void intToAscii(int num,char *output);
char * rIntToAscii(int num, char * result);
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
int len(char * str);
static void itoa(char *buf, int base, int d);
void itoa(unsigned int num, char * output);
