#include <stdio.h>
#include <stdarg.h>
#include <syscalls.h>
#include <stdint.h>
#include <file.h>
#include <folder.h>

extern "C" FILE *fopen(char *path, char *mode)
{
    void *a;
    uint32_t *addr;

    CALL_SYS3(a, FOPEN, path, mode, addr);

    FILE *file = (FILE *)(addr[0]);

    return (FILE *)addr[0];
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
    void *a;
    int ret;
    int off[1];
    int s[1];

    off[0] = size;
    s[1] = n;

    CALL_SYS4(ret, WRITE_FILE, buf, off, s, file->node->id);

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
    int ret;

    CALL_SYS4(ret, READ_FILE, buffer, size, n, fileno(file));
    file->ptr += size * n;

    return ret;
}

extern "C" int fgetc(FILE *file)
{
    char buf[1];
    fread(buf, 1, 1, file);

    if (file->ptr == file->node->size)
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
            w = file->node->size;
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
