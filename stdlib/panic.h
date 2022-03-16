#include <va_list.h>

void panic(char * msg, char * file, char * function, int line, ...);
void traceback(int limit);

#define PANIC(msg, ...) panic(msg, (char *)__FILE__, (char *)__PRETTY_FUNCTION__, __LINE__);
