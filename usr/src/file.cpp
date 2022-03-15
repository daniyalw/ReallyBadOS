#include <stdio.h>
#include <stdarg.h>
#include <syscalls.h>
#include <stdint.h>
#include <file.h>

extern "C" FILE *fopen(char *path)
{
    void *a;
    uint32_t *addr;

    asm volatile("int $48" : "=a" (a) : "0" (FOPEN), "b" (path), "c" (addr));

    FILE *file = (FILE *)(addr[0]);

    return (FILE *)addr[0];
}

extern "C" void fclose(FILE *file)
{
    void *a;

    asm volatile("int $48" : "=a" (a) : "0" (FCLOSE), "b" (file));
}

extern "C" int fexec(char *contents)
{
    void *a;
    int *ret;

    asm volatile("int $48" : "=a" (a) : "0" (FEXEC), "b" (contents), "c" (ret));

    return ret[0];
}

extern "C" void ls(char *path)
{
    void *a;
    asm volatile("int $48" : "=a" (a) : "0" (FLS), "b" (path));
}

extern "C" int mkfile(char *name, char *dir, char *contents)
{
    void * a;
    int res[1];
    asm volatile("int $48" : "=a" (a) : "0" (MKFILE), "b" (name), "c" (dir), "d" (contents), "S" (res));

    return res[0];
}

extern "C" int fwrite(char *buf, int offset, int size, FILE *file)
{
    void *a;
    int res[1];
    int off[1];
    int s[1];

    off[0] = offset;
    s[1] = size;
    asm volatile("int $48" : "=a" (a) : "0" (WRITE_FILE), "b" (buf), "c" (off), "d" (s), "S" (file->node->id), "D" (res));

    return res[0];
}

extern "C" void append_file(char *name, char *contents)
{
    void *a;
    asm volatile("int $48" : "=a" (a) : "0" (APPEND_FILE), "b" (name), "c" (contents));
}


extern "C" char * fread(char *buffer, int offset, int size, FILE *file)
{
    int off[1];
    void *a;
    int s[1];

    off[0] = offset;
    s[0] = size;

    asm volatile("int $48" : "=a" (a) : "0" (READ_FILE), "b" (buffer), "c" (off), "d" (s), "S" (file->node->id));

    return buffer;
}

extern "C" int fgetc(FILE *file)
{
    char c = fread("", file->ptr, 1, file)[0];
    file->ptr++;

    if (file->ptr == file->node->size)
        file->eof = EOF;

    return c;
}

extern "C" int feof(FILE *file)
{
    return file->eof < 0;
}

extern "C" char *fgets(char *str, int n, FILE *file)
{
    return fread(str, file->ptr, n, file);
}

extern "C" int fgetpos(FILE *file, fpos_t *pos)
{
    *pos = file->ptr;

    return *pos;
}

extern "C" int fseek(FILE *file, int offset, int w)
{
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

extern "C" void rewind(FILE *file)
{
    fseek(file, 0, 0);
}

extern "C" int fsetpos(FILE *file, fpos_t *pos)
{
    file->ptr = *pos;
    return file->ptr;
}
