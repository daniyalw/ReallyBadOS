#include <sys/syscall/syscall.h>
#include <random.h>
#include <sys/power/power.h>
#include <mouse/cursor.h>
#include <filesystem.h>
#include <shell/shell.h>

void set_string(char * string, char * newvalue)
{
    for (int z = 0; z < len(string); z++)
    {
        string[z] = ' ';
    }

    for (int z = 0; z < len(newvalue); z++)
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

typedef struct
{
    char name[20];
    char version[10];
    bool dev;
} info_t;

void s_info(info_t *info)
{
    strcpy(info->name, (char *)System::SYSTEM);
    strcpy(info->version, (char *)System::VERSION);
    info->dev = System::dev;
}

void s_file(char *path, uint32_t *addr)
{
    if (path[0] != '/')
    {
        path = get("", "/%s", path);
    }

    FILE *file = fopen(path);
    uint32_t address;

    if (file == NULL || file->null)
        address = NULL;
    else
        address = (uint32_t)file;

    addr[0] = address;
}

void s_fclose(FILE *file)
{
    fclose(file);
}

void exec_file(char *contents, int *ret)
{
    int r = execute_script(contents);

    ret[0] = r;
}

void s_ls(char *path)
{
    // automatically attach slash if user does not do it
    if (path[0] != '/')
    {
        path = get("", "/%s/", path);
    }
    fs_ls(path);
}

void s_create_folder(char *folder, char *parent_dir, int *result)
{
    result[0] = mkdir(folder, parent_dir);
}

void s_create_file(char *name, char *folder, char *contents, int *res)
{
    res[0] = create_file(name, folder, contents);
}

DEFN_SYSCALL1(print, 0, char *);

DEFN_SYSCALL1(s_putchar, 1, char);

DEFN_SYSCALL1(get_num, 2, int *);

DEFN_SYSCALL1(s_get_time, 3, time_t *);

DEFN_SYSCALL3(s_putpixel, 4, int, int, int);

DEFN_SYSCALL0(s_update_mouse, 5);

DEFN_SYSCALL1(s_info, 6, info_t*);

DEFN_SYSCALL2(s_file, 7, char*, uint32_t*);

DEFN_SYSCALL1(s_fclose, 8, FILE*);

DEFN_SYSCALL2(exec_file, 9, char*, int*);

DEFN_SYSCALL1(s_ls, 10, char*);

DEFN_SYSCALL3(s_create_folder, 11, char*, char*, int*);

DEFN_SYSCALL4(s_create_file, 12, char*, char*, char*, int);

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
    syscalls[syscall_count] = func;
    syscall_count++;
}

void syscall_print_syscalls()
{
    for (int z = 0; z < syscall_count; z++)
        printf("%x\n", syscalls[z]);

    printf("===\n");

    printf("%x\n%x\n%x\n%x\n%x\n%x\n%x\n", (void *)print, (void *)s_putchar, (void *)get_num, (void *)s_get_time,
                                        (void *)s_putpixel, (void *)s_update_mouse, (void *)s_info);
}

void syscall_handler(registers_t regs)
{
   if (regs.eax >= syscall_count)
   {
       error("Syscall outside of initialized syscalls range.\n");
       return;
   }

   void *location = syscalls[regs.eax];

   info("Syscall: %d\nLocations: %x:%x\n", regs.eax, location, (void *)s_info);

   //syscall_print_syscalls();

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
    syscall_append((void *)s_info);
    syscall_append((void *)s_file);
    syscall_append((void *)s_fclose);
    syscall_append((void *)exec_file);
    syscall_append((void *)s_ls);
    syscall_append((void *)s_create_folder);
    syscall_append((void *)s_create_file);
    Kernel::register_interrupt_handler(IRQ16, syscall_handler);
    Kernel::system_log("Syscalls initialized at interrupt 48!\n");
}

}
