void panic(char * msg, char * file, char * function, int line);

#define PANIC(msg) panic(msg, (char *)__FILE__, (char *)__FUNCTION__, __LINE__);
