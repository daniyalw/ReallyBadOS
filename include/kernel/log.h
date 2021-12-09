#pragma once

namespace Kernel {

char buffer[10000];
int buffer_size = 0;

void system_log(char * data);
char * get_log(char * buff);

}
