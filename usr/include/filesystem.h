#pragma once

#include <file.h>
#include <folder.h>

#ifdef __cplusplus
namespace Filesystem {

extern FILE *open(char *path, char *mode);
extern int read(char *buf, int sz, int n, FILE *file);
extern int write(char *buf, int sz, int n, FILE *file);
extern int file(char *name, char *parent);
extern int folder(char *name, char *parent);
extern void close(FILE *stream);

}
#endif
