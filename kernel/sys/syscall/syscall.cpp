#include <sys/syscall/syscall.h>
#include <random.h>
#include <sys/power/power.h>
#include <mouse/cursor.h>
#include <shell/shell.h>
#include <sys/log/log.h>
#include <filesystem/file.h>
#include <filesystem/dir.h>

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

void print(char * text, int *ret)
{
    ret[0] = printf(text);
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
    char *name;
    char *version;
    bool dev;
} info_t;

void s_info(info_t *info)
{
    info[0].name = (char *)System::SYSTEM;
    info[0].version = (char *)System::VERSION;
    info[0].dev = System::dev;
}

void s_file(char *path, uint32_t *addr)
{
    FILE *file = fopen(path);

    if (file == NULL)
        addr[0] = NULL;
    else
        addr[0] = (uint32_t)file;
}

void s_fclose(FILE *file)
{
    free(file);
}

void exec_file(char *contents, int *ret)
{

}

void s_ls(char *path)
{
    list_dir(path);
}

void s_create_folder(char *folder, char *parent_dir, int *result)
{
    result[0] = make_dir_user(folder, parent_dir);
}

void s_create_file(char *name, char *folder, char *contents, int *res)
{
    res[0] = create_file(name, folder, contents);
}

void s_write_file(char *contents, int *offset, int *size, FILE *file, int *res)
{
    if (file == NULL)
    {
        res[0] = NULL;
        return;
    }

    res[0] = fwrite(file, offset[0], size[0], contents);
}

void s_malloc(int *size, uint32_t *addr)
{
    addr[0] = malloc(size[0]);
}

void s_free(void *buf)
{
    free(buf);
}

void s_append_file(char *name, char *contents)
{
}

void s_read_file(char *buf, int *offset, int *size, FILE *file)
{
    if (file == NULL)
    {
        buf = NULL;
        return;
    }
}

// ----------------------------- //


void syscall_append(void *func)
{
    if (syscall_count >= max_syscalls)
    {
        log::warning("Max syscalls reached!");
        return;
    }
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

void syscall_handler(registers_t *regs)
{
   if (regs->eax >= syscall_count)
   {
      log::error("Syscall outside of initialized syscalls range.");
       return;
   }

   void *location = syscalls[regs->eax];

   log::info("Syscall: %d\nLocations: %x:%x", regs->eax, location, (void *)s_info);

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
   " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (location));
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
    syscall_append((void *)s_write_file);
    syscall_append((void *)s_malloc);
    syscall_append((void *)s_free);
    syscall_append((void *)s_append_file);
    syscall_append((void *)s_read_file);
    Kernel::register_interrupt_handler(IRQ16, syscall_handler);
    log::info("Syscalls initialized at interrupt 48!");
}

}
