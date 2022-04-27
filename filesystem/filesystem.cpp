#include "file.h"

namespace Filesystem {

FILE *open(char *name, char *mode) {
    return fopen(name, mode);
}

int read(char *buf, int sz, int n, FILE *file) {
    return fread(buf, sz, n, file);
}

int write(char *buf, int sz, int n, FILE *file) {
    return fwrite(buf, sz, n, file);
}

int printf(FILE *file, char *buf) {
    return fprintf(file, buf);
}

}
