#pragma once

#include <va_list.h>

namespace Kernel {

char buffer[10000];
int buffer_size = 0;

void system_log_char(char ch);
void system_log_string(char *text, va_list va);
void system_log(char * text, ...);
char * get_log(char * buff);

}
