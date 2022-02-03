#include <kernel/kernel.h>

extern "C" {
    extern void jmp_somewhere(unsigned int place);
    extern void main();
    extern unsigned int stack_top;
    extern unsigned int kernel_end;
    extern unsigned int read_eip();
}

//#define DEBUG
//#define GRAPHICS

#include <cpuid.h>

#ifdef GRAPHICS
#include "sys/background.cpp"
#endif

#include <sys/log/log.h>
#include <stdint.h>
#include <map.h>
#include <video/ssfn.h>
#include <sys/multiboot.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <video/video.h>
#include <mouse/mouse.h>
#include <sys/time/time.h>
#include <keyboard/keyboard.h>
#include <video/graphics.h>
#include <list.h>
#include <macros.h>
#include <system.h>
#include <assert.h>
#include <va_list.h>
#include <operator.h>

#include "sys/io.cpp"
#include "../stdlib/math.cpp"
#include "sys/power/shutdown.cpp"
#include "sys/power/reboot.cpp"
#include "sys/cpu/info.cpp"
#include "../drivers/mouse/cursor.cpp"
#include "../stdlib/string.cpp"
#include "../drivers/video/video.cpp"
#include "sys/log/log.cpp"
#include "sys/descriptors/gdt.cpp"
#include "sys/descriptors/idt.cpp"
#include "sys/descriptors/isr.cpp"
#include "sys/descriptors/interrupts.cpp"
#include "../drivers/sound/sound.cpp"
#include "shell/shell.cpp"
#include "../drivers/video/font.cpp"
#include "../drivers/video/graphics.cpp"
#include "../drivers/keyboard/keyboard.cpp"
#include "../drivers/mouse/mouse.cpp"
#include "sys/time/time.cpp"
#include "sys/time/timer.cpp"
#include "../stdlib/colors.cpp"
#include "../drivers/video/bga.cpp"
#include "sys/serial.cpp"
#include "sys/syscall/syscall.cpp"
#include "../drivers/video/blur.cpp"
#include "sys/panic.cpp"
#include "sys/cpu/halt.cpp"
#include "sys/syscall/syscalls.h" // this is external
#include "../fs/tar.cpp"
#include "../fs/ramdisk.cpp"
#include "sys/syscall/usermode.cpp"
#include "../drivers/disk/ata.cpp"
#include "sys/mem/mem.cpp"
#include "sys/mem/block.cpp"
#include "sys/mem/malloc.cpp"
#include "sys/mem/free.cpp"
#include "sys/mem/realloc.cpp"
#include "sys/mem/calloc.cpp"
#include "sys/multitasking/cooperative.cpp"
#include "../stdlib/tree.cpp"
#include "sys/pci/pci.cpp"
#include "../drivers/keyboard/getch.cpp"
#include "../drivers/keyboard/scanf.cpp"
#include "../gui/gui.h"
#include "../exec/elf.cpp"
#include "../fs/utilities.cpp"
#include "../exec/argparse.cpp"

using namespace Filesystem;
using namespace Ramdisk;
using namespace Time;
using namespace Cooperative;

void detect_net()
{
    PCIDevice *device = find_device(0x10EC, 0x8139);

    if (device == NULL)
    {
        error("could not detect RTL8139 network card\n");
    }
    else
    {
        info("detected RTL8139 network card!\n");
    }
}

void login()
{
    uint8_t *login;
    ata_read(login, 0, 1);

    char username[64];
    char password[64];
    int b = 0;

    for (int z = 6; z < 64; z++)
    {
        if ((char)login[z] == '=')
        {
            b = z + 1;
            break;
        }
        username[z-6] = (char)login[z];
    }

    for (int z = b; z < 128; z++)
    {
        if ((char)login[z] == 0) break;
        password[z-b] = (char)login[z];
    }

    while (true)
    {
        printf("Username: ");
        char *user = scanf();

        printf("Password: ");
        char *pass = scanf();

        if (strcmp(user, username) && strcmp(pass, password))
        {
            free(user);
            free(pass);
            printf("Logged in!\n");
            break;
        }
        else
        {
            printf("Incorrect username or password.\n");
#ifdef DEBUG
            printf("Username: %s ::: Password: %s\n", username, password);
#endif
        }
    }
}

void new_user()
{
    char *username;
    char *password;

    while (true)
    {
        printf("Hi!\n\nEnter a username: ");
        username = scanf();

        if (contains(username, '='))
            printf("Username cannot contain '='!\n");
        else
            break;
    }

    while (true)
    {
        printf("Now enter a password: ");
        password = scanf();

        if (contains(password, '='))
            printf("Password cannot contain '='!\n");
        else
            break;
    }

    printf("Registering...\n");

    ata_write_one(0, (uint8_t *)std::get("", "LOGIN:%s=%s\0", username, password));

    free(username);
    free(password);

    printf("Registered!\n");
}

void detect_user()
{
    uint8_t *bytes;
    ata_read(bytes, 0, 1);

    char first_six[7];

    for (int z = 0; z < 6; z++)
        first_six[z] = (char)bytes[z];

    first_six[6] = 0;

    if (strcmp(first_six, "LOGIN:"))
        login();
    else
        new_user();
}

extern "C" void kernel_main(multiboot_info_t *mbd, unsigned int magic, uint stack) {
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        Kernel::system_log("Invalid magic.\n"); // we could use printf, but that's text-mode only
        return; // stop any more kernel code running since it's not multiboot
    }

    Kernel::system_log("Entered CeneOS kernel.\n");

    create_folder("dev");
    create_folder("usr");
    create_folder("trash");
    create_folder("apps");

    Kernel::init_serial(SERIAL_PORT);
    Kernel::init_logging();
    Kernel::init_isr();
    Kernel::init_gdt();
    Kernel::init_tss();
    Kernel::init_sound();
    Kernel::read_rtc();
    Kernel::init_timer(1000);
    Kernel::init_keyboard(false, "/");
    Kernel::init_mouse();
    Kernel::init_syscalls();

    init_vga();

    u32 location = *((u32*)mbd->mods_addr); // load modules with GRUB

    uint32_t beginning = location;

    Tar tar;

    // parse the tar file
    tar.parse(location);

    // loop through the files in the tar block and create the files in the vfs
    for (int z = 0; z < tar.block_count; z++)
    {
        beginning += 512;
        beginning += tar.blocks[z].size;
    }

    // for graphics
#ifdef GRAPHICS

    Graphic::init_graphics(mbd);

    init_mem(mbd, beginning);

    for (int z = 0; z < tar.block_count; z++)
        create_file(tar.blocks[z].name, "usr", tar.blocks[z].contents, tar.blocks[z].size * sizeof(char));

    Graphic::redraw_background_picture(array);

    Window win;
    Widget w;
    w.setx(120);
    w.sety(120);
    w.padding = 10;
    w.set_draw(test_draw);
    win.widgets[0] = w;
    win.widget_count++;
    win.set_coords(100, 100);
    win.set_dimensions(100, 100);

    win.draw();

    //draw_string(100, 100, Graphic::rgb(255, 255, 255), "Framebuffer data:\n\tHeight: %d\n\tWidth: %d\n\tBPP: %d", height, width, bpp);

    //int ret = gui_notification("About", "CeneOS is just another hobby operating system!\n2022");

    Graphic::blit_changes();

    while (true);

#else

    init_mem(mbd, beginning);

    for (int z = 0; z < tar.block_count; z++)
    {
        if (endswith(tar.blocks[z].name, "o"))
        {
            create_file(tar.blocks[z].name, "apps", tar.blocks[z].contents, tar.blocks[z].size * sizeof(char));
        }
        else
        {
            create_file(tar.blocks[z].name, "usr", tar.blocks[z].contents, tar.blocks[z].size * sizeof(char));
        }
    }

    /*
    putchar(' ');
    text_x--;

    char *argv[] = {"Hello!", "Hi!"};
    int result = load_app_from_file("/usr/test.cos", 2, argv);

    if (!result)
        printf("Exit returned successfully!\n");
    else
        printf("Application returned unsuccessfully.\n");
    */

    scan_buses();

    time_t time = get_time();

    uint8_t *bytes;
    bytes = ata_init(bytes);

    printf("CeneOS Shell\n");

    printf("%s, %s %d\n", weekdays[time.wd-1], months[time.m-1], time.d);

    if (time.min < 10)
        printf("%d:0%d %s\n", time.h, time.min, (char *)(time.pm ? "PM" : "AM"));
    else
        printf("%d:%d %s\n", time.h, time.min, (char *)(time.pm ? "PM" : "AM"));

    switch_to_user_mode();
    shell();

    Kernel::serial_write_string("\n");
    while (true) {}

#endif
}
