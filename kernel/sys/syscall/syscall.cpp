#include <sys/syscall/syscall.h>
#include <random.h>
#include <sys/power/power.h>
#include <mouse/cursor.h>
#include <fs.h>

void set_string(char * string, char * newvalue)
{
    for (int z = 0; z < std::len(string); z++)
    {
        string[z] = ' ';
    }

    for (int z = 0; z < std::len(newvalue); z++)
    {
        string[z] = newvalue[z];
        string[z+1] = 0;
    }
}

// ----------------------------- //

void print(char * text)
{
    printf(text);
}

void s_putchar(char text)
{
    putchar(text);
}

void get_num(int * num)
{
    num[0] = get_random_number();
}

void s_get_time(time_t time[1])
{
    time[0] = Time::get_time();
}

void s_putpixel(int x, int y, int color)
{
    Graphic::SetPixel(x, y, color);
}

void test(int num)
{
    num = 105;
}

void s_update_mouse()
{
    next_char();
    Kernel::update_hardware_cursor(text_x, text_y);
}

void s_test()
{
    printf("Hello!");
}

void s_fopen(char * name, FILE *f)
{
    path_t path = parse_name(name);
    FILE * file;
    FILE orig = get_file(path.filename, path.foldername);

    file->name = orig.name;
    file->contents = orig.contents;
    file->path = orig.path;
    file->parent = orig.parent;
    file->id = orig.id;
    file->size = orig.size;
    file->null = orig.null;
    file->write = orig.write;
    file->read = orig.read;

    f = file;
}

DEFN_SYSCALL1(print, 0, char *);

DEFN_SYSCALL1(s_putchar, 1, char);

DEFN_SYSCALL1(get_num, 2, int *);

DEFN_SYSCALL1(s_get_time, 3, time_t *);

DEFN_SYSCALL3(s_putpixel, 4, int, int, int);

DEFN_SYSCALL0(s_update_mouse, 5);

DEFN_SYSCALL0(s_test, 6);

DEFN_SYSCALL2(s_fopen, 7, char*, FILE*);

// ----------------------------- //

void sys_putchar(char text)
{
    syscall_s_putchar(text);
}

void sys_print(char *text, ...)
{
    va_list va;
    va_start(va, text);
    vprintf(text, va);
    va_end(va);
}

int sys_rand()
{
    int a[1];
    syscall_get_num(a);
    return a[0];
}

time_t sys_get_time()
{
    time_t time[1];
    syscall_s_get_time(time);
    return time[0];
}

void sys_putpixel(int x, int y, int color)
{
    syscall_s_putpixel(x, y, color);
}

// ----------------------------- //


void syscall_append(void *func)
{
    syscalls.push_back((void *)func);
}

void syscall_handler(registers_t regs)
{
   if (regs.eax >= syscalls.size())
   {
       error("Syscall outside of initialized syscalls range.\n");
       return;
   }

   void *location = syscalls.get(regs.eax);

   int ret;

   asm volatile (" \
     push %1; \
     push %2; \
     push %3; \
     push %4; \
     push %5; \
     call *%6; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
   " : "=a" (ret) : "r" (regs.edi), "r" (regs.esi), "r" (regs.edx), "r" (regs.ecx), "r" (regs.ebx), "r" (location));
   //regs.eax = ret;
}

namespace Kernel {

void init_syscalls()
{
    syscall_append((void *)print);
    syscall_append((void *)s_putchar);
    syscall_append((void *)get_num);
    syscall_append((void *)s_get_time);
    syscall_append((void *)s_putpixel);
    syscall_append((void *)s_update_mouse);
    syscall_append((void *)s_test);
    syscall_append((void *)s_fopen);
    Kernel::register_interrupt_handler(IRQ16, syscall_handler);
    Kernel::system_log("Syscalls initialized at interrupt 48!\n");
}

}
