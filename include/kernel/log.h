#pragma once

namespace Kernel {

char sys_log[10000];
int sys_log_size = 0;

void system_log(char * data);

}
