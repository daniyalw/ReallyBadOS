#include <stdio.h>
#include <stdarg.h>
#include <syscalls.h>
#include <stdint.h>
#include <file.h>
#include <folder.h>
#include <errno.h>

extern "C" FILE *fopen(char *path, char *mode)
{
    void *a;
    uint32_t addr;

    CALL_SYS3(a, FOPEN, path, mode, &addr);

    FILE *file = (FILE *)addr;

    if (!file) {
        errno = ENOENT;
        return NULL;
    }

    return file;
}

extern "C" void fclose(FILE *file)
{
    void *a;

    CALL_SYS1(a, FCLOSE, file);
}

extern "C" int fexec(char *contents)
{
    void *a;
    int ret;

    CALL_SYS1(ret, FEXEC, contents);

    return ret;
}

extern "C" void ls(char *path)
{
    void *a;
    CALL_SYS1(a, FLS, path);
}

extern "C" int mkfile(char *name, char *dir, char *contents)
{
    int ret;
    CALL_SYS3(ret, MKFILE, name, dir, contents);

    return ret;
}

extern "C" int fwrite(char *buf, int size, int n, FILE *file)
{
    int ret = file->node->write(file->node, file->ptr, size * n, buf);
    file->ptr += size * n;

    return ret;
}

extern "C" void append_file(char *name, char *contents)
{
    void *a;
    CALL_SYS2(a, APPEND_FILE, name, contents);
}

extern "C" int fileno(FILE *stream) {
    return stream->node->id;
}


extern "C" int fread(char *buffer, int size, int n, FILE *file)
{
    int ret = file->node->read(file->node, file->ptr, size * n, buffer);
    file->ptr += size * n;

    if (file->ptr >= fsize(file)) {
        file->eof = EOF;
    }

    return ret;
}

extern "C" int fgetc(FILE *file)
{
    char buf[1];
    fread(buf, 1, 1, file);

    if (file->ptr == file->node->get_size(file->node))
        file->eof = EOF;

    return buf[0];
}

extern "C" int feof(FILE *file)
{
    return file->eof < 0;
}

extern "C" char *fgets(char *str, int n, FILE *file)
{
    fread(str, 1, n, file);
    return str;
}

extern "C" int fgetpos(FILE *file, fpos_t *pos)
{
    *pos = file->ptr;

    return *pos;
}

extern "C" int fseek(FILE *file, int offset, int whence)
{
    int w = 0;

    switch (whence)
    {
        case SEEK_SET:
            w = 0;
            break;
        case SEEK_CUR:
            w = file->ptr;
            break;
        case SEEK_END:
            w = file->node->get_size(file->node);
            break;
    }

    if (file == NULL)
        return 1;

    file->ptr = offset + w;
    return 0;
}

extern "C" int ftell(FILE *file)
{
    int offset[1];
    offset[0] = fgetpos(file, offset);
    return offset[0];
}

extern "C" int fsize(FILE *file)
{
    fseek(file, 0, SEEK_END);
    int ret = ftell(file);
    fseek(file, 0, 0);
    return ret;
}

extern "C" void rewind(FILE *file)
{
    fseek(file, 0, 0);
}

extern "C" int fsetpos(FILE *file, fpos_t *pos)
{
    file->ptr = *pos;
    return file->ptr;
}

extern "C" int fprintf(FILE *stream, char *format, ...) {
    va_list va;
    int length = strlen(format);
    char out[length + 100];

    memset(out, 0, length + 100);
    va_start(va, format);

    vsprintf(out, format, va);

    va_end(va);

    return fwrite(out, 1, strlen(out), stream);
}

namespace Filesystem {
    FILE *open(char *path, char *mode) {
        return fopen(path, mode);
    }

    int read(char *buf, int sz, int n, FILE *file) {
        return fread(buf, sz, n, file);
    }

    int write(char *buf, int sz, int n, FILE *file) {
        return fwrite(buf, sz, n, file);
    }

    int file(char *name, char *parent) {
        return mkfile(name, parent, NULL);
    }

    int folder(char *name, char *parent) {
        return mkdir(name, parent);
    }

    void close(FILE *stream) {
        fclose(stream);
    }
}
