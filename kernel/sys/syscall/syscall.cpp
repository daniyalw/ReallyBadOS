#include <sys/syscall/syscall.h>
#include <random.h>
#include <sys/power/power.h>
#include <mouse/cursor.h>
#include <shell/shell.h>
#include <sys/log/log.h>
#include <filesystem/file.h>
#include <filesystem/dir.h>
#include <filesystem/env.h>

void set_string(char * string, char * newvalue) {
    for (int z = 0; z < strlen(string); z++) {
        string[z] = ' ';
    }

    for (int z = 0; z < len(newvalue); z++) {
        string[z] = newvalue[z];
        string[z+1] = 0;
    }
}

// ----------------------------- //

int print(char * text) {
    return printf(text);
}

int s_putchar(char text) {
    putchar(text);
    return 0;
}

int get_num() {
    return get_random_number();
}

int s_get_time(time_t time[1]) {
    time[0] = Time::get_time();
    return 0;
}

int s_putpixel(int x, int y, int color) {
    Graphic::SetPixel(x, y, color);
    return 0;
}

int s_update_mouse() {
    next_char();
    Kernel::update_hardware_cursor(text_x, text_y);
    return 0;
}

int s_test(int chocolate) {
    chocolate = 5;
    printf("Hello!");
    return 9;
}

typedef struct {
    char *name;
    char *version;
    char *vendor;
    char cpu_name[30];
    bool dev;
    bool hypervisor;
    int model;
} info_t;

int s_info(info_t *info) {
    memset(info[0].cpu_name, 0, 30);

    info[0].name = (char *)System::SYSTEM;
    info[0].version = (char *)System::VERSION;
    info[0].hypervisor = System::hypervisor();
    info[0].vendor = System::get_vendor();
    info[0].model = System::get_model();
    System::get_cpu_name(info[0].cpu_name);
    info[0].dev = System::dev;

    return 0;
}

int s_file(char *path, char *mode, uint32_t *addr) {
    FILE *file = fopen(find_fixed(path), mode);

    if (!file) {
        addr[0] = NULL;
        return 1;
    }

    addr[0] = (uint32_t)file;
    return 0;
}

int s_fclose(FILE *file) {
    free(file);
    return 0;
}

int exec_file(char *contents) {
    // TODO: actually execute file
    return 0;
}

int s_ls(char *path) {
    list_dir(path);
    return 0;
}

int s_create_folder(char *folder, char *parent_dir) {
    return make_dir_user(folder, find_fixed(parent_dir));
}

int s_create_file(char *name, char *folder, char *contents) {
    return create_file(name, find_fixed(folder), contents);
}

int s_write_file(char *contents, int *size, int *n, int fd) {
    if (fd < 0)
        return 1;

    fs_node_t *node = find_node(fd);

    if (node == NULL)
        return 1;

    FILE *file = fopen(node->path, "r");

    if (file == NULL)
        return 1;

    int ret = fwrite(contents, size[0], n[0], file);

    fclose(file);

    return ret;
}

int s_malloc(int *size, uint32_t *addr) {
    addr[0] = malloc(size[0]);
    return 0;
}

int s_free(void *buf) {
    free(buf);
    return 0;
}

int s_append_file(char *name, char *contents) {
    return 0;
}

int s_read_file(char *buf, int *size, int *n, int fd) {
    int ret = 0;

    if (fd < 0) {
        return 1;
    }

    fs_node_t *node = find_node(fd);

    if (node == NULL) {
        return 1;
    }

    FILE *file = fopen(node->path, "r");

    if (file == NULL)
        return 1;
    else
        ret = fread(buf, size[0], n[0], file);

    fclose(file);

    return ret;
}

int s_create_proc(char *name, uint32_t addr) {
    return Kernel::CPU::create_thread(name, addr);
}

int s_yield() {
    Kernel::CPU::yield();
    return 0;
}

int s_exit(int ret) {
    Kernel::CPU::exit(ret);
    return 0;
}

int s_getenv(char *name, char *env) {
    set_string(env, getenv(name));
    return 0;
}

int s_setenv(char *name, char *val) {
    return setenv(name, val, 1);
}

int s_realloc(void *buf, int newsize) {
    return realloc(buf, newsize);
}

int s_get_tag(int tag, char *out) {
    set_string(out, find_tag(tag));
    return 0;
}

int s_send_ipc(int to, char *msg) {
    return ipc::send_msg(to, msg);
}

int s_recv_msg(Message **msgs) {
    return ipc::find_messages(msgs);
}

int s_name_from_pid(int pid, char *buf) {
    set_string(buf, name_from_pid(pid));
    return 0;
}

int s_pid_from_name(int *pid, char *buf) {
    pid[0] = pid_from_name(buf);
    return 0;
}

// ----------------------------- //


void syscall_append(void *func) {
    if (syscall_count >= max_syscalls) {
        log::warning("Max syscalls reached!");
        return;
    }

    syscalls[syscall_count] = func;
    syscall_count++;
}

void syscall_print_syscalls() {
    for (int z = 0; z < syscall_count; z++)
        printf("%x\n", syscalls[z]);

    printf("===\n");

    printf("%x\n%x\n%x\n%x\n%x\n%x\n%x\n", (void *)print, (void *)s_putchar, (void *)get_num, (void *)s_get_time,
                                        (void *)s_putpixel, (void *)s_update_mouse, (void *)s_info);
}

void syscall_handler(registers_t *regs) {
    asm("cli");

    if (regs->eax >= syscall_count) {
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
    regs->eax = ret;
    asm("sti");
}

namespace Kernel {

void init_syscalls() {
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
    syscall_append((void *)s_create_proc);
    syscall_append((void *)s_yield);
    syscall_append((void *)s_exit);
    syscall_append((void *)s_getenv);
    syscall_append((void *)s_setenv);
    syscall_append((void *)s_realloc);
    syscall_append((void *)s_get_tag);
    syscall_append((void *)s_send_ipc);
    syscall_append((void *)s_recv_msg);
    syscall_append((void *)s_name_from_pid);
    syscall_append((void *)s_pid_from_name);
    Kernel::CPU::register_interrupt_handler(IRQ16, syscall_handler);
    log::info("Syscalls initialized at interrupt 48!");
}

}
