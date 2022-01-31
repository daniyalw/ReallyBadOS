#include <itoa.h>
#include <stdarg.h>
#include <string.h>
#include <printf.h>
#include <syscalls.h>

extern "C" char * vnprintf(char *s, int max, char *format, va_list va)
{
    int sz = 0;
    char buffer[20];

    for (int z = 0; z < max; z++)
    {
        if (format[z] == '%')
        {
            z++;
            if (format[z] == 's')
            {
                char *str = (char *)va_arg(va, char*);

                for (int b = 0; b < strlen(str); b++)
                {
                    s[sz] = str[b];
                    sz++;
                }
            }
            else if (format[z] == 'c')
            {
                int ch = (int)va_arg(va, int);
                s[sz] = ch;
                sz++;
            }
            else if (format[z] == 'd')
            {
                int i = (int)va_arg(va, int);
                for (int b = 0; b < 20; b++) buffer[b] = 0;
                itoa(buffer, 'd', i);
                for (int b = 0; b < strlen(buffer); b++)
                {
                    s[sz] = buffer[b];
                    sz++;
                }
            }
            else if (format[z] == 'x')
            {
                int arg = (int)va_arg(va, int);
                for (int b = 0; b < 20; b++) buffer[b] = 0;
                itoa(buffer, 'x', arg);
                for (int b = 0; b < strlen(buffer); b++)
                {
                    s[sz] = buffer[b];
                    sz++;
                }
            }
            else
            {
                s[sz] = '%';
                sz++;
                s[sz] = format[z];
                sz++;
            }
        }
        else
        {
            s[sz] = format[z];
            sz++;
        }
    }

    s[sz] = 0;

    return s;
}

extern "C" char * vsprintf(char *s, char *format, va_list va)
{
    return vnprintf(s, strlen(format), format, va);
}

extern "C" void vprintf(char *f, va_list va)
{
    printf(vsprintf("", f, va));
}

extern "C" void sprintf(char *s, char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    vsprintf(s, fmt, va);
    va_end(va);
}

extern "C" void snprintf(char *s, int max, char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    vnprintf(s, max, fmt, va);
    va_end(va);
}

extern "C" void putchar(char text) {
    void * a;
    asm volatile("int $48" : "=a" (a) : "0" (PUTCHAR), "b" (text));
}

extern "C" void printf(char *a, ...)
{
    va_list va;
    va_start(va, a);
    char * out = vsprintf("", a, va);
    va_end(va);

    for (int z = 0; z < strlen(out); z++)
        putchar(out[z]);
}

void test_print()
{
    printf("Hello!");
}
