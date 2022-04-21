#include "file.h"

namespace Filesystem {

FILE *open(char *name, char *mode) {
    return fopen(name, mode);
}

int read(char *buf, int sz, int n, FILE *file) {
    return fread(file, sz, n, buf);
}

int write(char *buf, int sz, int n, FILE *file) {
    return fwrite(file, sz, n, buf);
}

int printf(FILE *file, char *buf) {
    return fprintf(file, buf);
}

}
