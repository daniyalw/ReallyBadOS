#include <sys/syscall/syscall.h>
#include <random.h>
#include <sys/power/power.h>
#include <mouse/cursor.h>
#include <filesystem.h>
#include <shell/shell.h>
#include <sys/log/log.h>

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
    int id = find_id(path);
    if (id < 0)
        printf("Invalid folder/file!\n");
    else
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

void s_write_file(FILE *file, char *contents, int *res)
{
    if (file == NULL || file->null)
    {
        log::error("File '%s' not found", file->node.path);
        res[0] = 1;
    }
    else
    {
        res[0] = 0;
        file->write(contents);
    }
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
    FILE *file = fopen(name);

    if (file == NULL)
        return;

    node_write_append(file->node.id, contents);

    fclose(file);
}

void s_read_file(FILE *file, char *buf)
{
    if (file == NULL)
    {
        log::warn("file is null: %s", file->node.path);
        return;
    }

    strcpy(buf, file->read(buf));
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

void syscall_handler(registers_t regs)
{
   if (regs.eax >= syscall_count)
   {
      log::error("Syscall outside of initialized syscalls range.");
       return;
   }

   void *location = syscalls[regs.eax];

   log::info("Syscall: %d\nLocations: %x:%x", regs.eax, location, (void *)s_info);

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
    syscall_append((void *)s_write_file);
    syscall_append((void *)s_malloc);
    syscall_append((void *)s_free);
    syscall_append((void *)s_append_file);
    syscall_append((void *)s_read_file);
    Kernel::register_interrupt_handler(IRQ16, syscall_handler);
    log::info("Syscalls initialized at interrupt 48!");
}

}
