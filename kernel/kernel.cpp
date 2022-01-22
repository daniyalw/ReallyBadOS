int text_x = 0;
int text_y = 0;
int cx = 0;
int cy = 0;
bool booted = false;
int back_buffer[1024*768]; // back buffer for gui
//unsigned int initial_stack;
// this is the /> _
char current_display[128];
int current_display_len = 0;

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

#include <kernel/log.h>
#include <stdint.h>
#include <map.h>
#include <drivers/video/ssfn.h>
#include <kernel/multiboot.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <drivers/video/video.h>
#include <drivers/mouse/mouse.h>
#include <time.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/video/graphics.h>
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
#include "sys/interrupts/idt.cpp"
#include "sys/interrupts/isr.cpp"
#include "sys/interrupts/interrupts.cpp"
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
#include "../gui/notification.cpp"
#include "sys/syscall/syscalls.h" // this is external
#include "../fs/tar.cpp"
#include "../fs/ramdisk.cpp"
#include "../gui/gui.cpp"
#include "../gui/label.cpp"
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
#include "sys/pci.cpp"
#include "../drivers/keyboard/getch.cpp"
#include "../drivers/keyboard/scanf.cpp"

using namespace Filesystem;
using namespace Ramdisk;
using namespace Time;
using namespace Cooperative;

extern "C" void kernel_main(multiboot_info_t *mbd, unsigned int magic, uint stack) {
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        Kernel::system_log("Invalid magic.\n"); // we could use printf, but that's text-mode only
        return; // stop any more kernel code running since it's not multiboot
    }

    Kernel::system_log("Entered CeneOS kernel.\n");

    create_folder("dev");
    create_folder("usr");

    Kernel::init_serial(SERIAL_PORT);
    Kernel::init_logging();
    Kernel::init_isr();
    Kernel::init_gdt();
    Kernel::init_tss();
    Kernel::init_sound();
    Kernel::read_rtc();
    Kernel::init_timer(1000);
    Kernel::init_keyboard(false, "");
    Kernel::init_mouse();
    Kernel::init_syscalls();

    // for graphics
#ifdef GRAPHICS

    u32 location = *((u32*)mbd->mods_addr);
    parse(location);

    for (int z = 0; z < block_count; z++)
        create_file(blocks[z].name, "usr", blocks[z].contents, blocks[z].size * sizeof(char));

    Graphic::init_graphics(mbd);

    Graphic::redraw_background_picture(array);

    draw_string("Hello!", 100, 100, white);

    //draw_string(100, 100, Graphic::rgb(255, 255, 255), "Framebuffer data:\n\tHeight: %d\n\tWidth: %d\n\tBPP: %d", height, width, bpp);

    //int ret = gui_notification("About", "CeneOS is just another hobby operating system!\n2022");

    Graphic::blit_changes();

    while (true);

#else

    u32 location = *((u32*)mbd->mods_addr); // load modules with GRUB

    uint32_t beginning = location;

    // parse the tar file
    parse(location);

    // loop through the files in the tar block and create the files in the vfs
    for (int z = 0; z < block_count; z++)
    {
        create_file(blocks[z].name, "usr", blocks[z].contents, blocks[z].size * sizeof(char));
        beginning += 512;
        beginning += blocks[z].size;
    }

    init_mem(mbd, beginning);

    init_vga();

    uint8_t *bytes;
    bytes = ata_init(bytes);

    /*
    switch_to_user_mode();

    shell();
    */

    scan_buses();

    time_t time = get_time();

    printf("%s, %s %d\n", weekdays[time.wd-1], months[time.m-1], time.d);

    if (time.min < 10)
        printf("%d:0%d %s", time.h, time.min, (char *)(time.pm ? "PM" : "AM"));
    else
        printf("%d:%d %s", time.h, time.min, (char *)(time.pm ? "PM" : "AM"));

    printf("\n");

    shell();

    Kernel::serial_write_string("\n");
    while (true) {}

#endif
}
