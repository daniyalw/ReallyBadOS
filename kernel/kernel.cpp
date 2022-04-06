#include <kernel/kernel.h>

extern "C" {
    extern void jmp_somewhere(unsigned int place);
    extern void main();
    extern unsigned int stack_top;
    extern unsigned int kernel_end;
}

//#define DEBUG
//#define GRAPHICS
#define DIV_BYTES 1048576 // for some reason this comes in useful

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
#include <net/arp.h>
#include <net/ethernet.h>
#include <tree.h>
#include <cxa.h>
#include <gui/coords.h>

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
#include "../drivers/video/bga.cpp"
#include "sys/serial.cpp"
#include "sys/syscall/syscall.cpp"
#include "../drivers/video/blur.cpp"
#include "sys/panic.cpp"
#include "sys/cpu/halt.cpp"
#include "sys/syscall/syscalls.h" // this is external
#include "../filesystem/tar.cpp"
#include "sys/syscall/usermode.cpp"
#include "../drivers/disk/ata.cpp"
#include "sys/mem/mem.cpp"
#include "sys/mem/block.cpp"
#include "sys/mem/malloc.cpp"
#include "sys/mem/free.cpp"
#include "sys/mem/realloc.cpp"
#include "sys/mem/calloc.cpp"
#include "sys/multitasking/task.cpp"
#include "../stdlib/tree.cpp"
#include "sys/pci/pci.cpp"
#include "../drivers/keyboard/getch.cpp"
#include "../drivers/keyboard/scanf.cpp"
#include "../gui/gui.h"
#include "../exec/elf.cpp"
#include "../exec/argparse.cpp"
#include "../stdlib/ctype.cpp"
#include "dev.cpp"
#include "../drivers/net/rtl.cpp"
#include "../net/ethernet.cpp"
#include "../net/utils.cpp"
#include "../net/arp.cpp"
#include "../drivers/disk/disk.cpp"
#include "../stdlib/utils.cpp"
#include "../filesystem/node.cpp"
#include "../filesystem/file.cpp"
#include "../filesystem/dir.cpp"
#include "../filesystem/test.cpp"
#include "../stdlib/errno.cpp"
#include "sys/multitasking/thread.cpp"
#include "sys/multitasking/scheduler.cpp"
#include "sys/multitasking/ipc.cpp"
#include "../filesystem/disk.cpp"
#include "../drivers/net/net.cpp"

#ifdef GRAPHICS
#include "../gui/widget.cpp"
#include "../gui/button.cpp"
#include "../gui/gui.cpp"
#include "../gui/label.cpp"
#include "../gui/window.cpp"
#include "../gui/entry.cpp"

void callback(UIObject *obj, Event *event) {
    if (strcmp(event->name, "keyboard") == 0) {
        log::info("Keyboard event.\n");
    } else if (strcmp(event->name, "mouse") == 0) {
        log::info("Mouse event.\n");
    } else {
        log::info("Unknown event of type: %d name: %s\n", event->type, event->name);
    }
}
#endif

using namespace Time;
using namespace Graphic;

extern "C" void kernel_main(multiboot_info_t *mbd, unsigned int magic, uint32_t stack) {
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        log::error("Invalid magic."); // we could use printf, but that's text-mode only
        return; // stop any more kernel code running since it's not multiboot
    }

    log::info("Entered %s kernel.", System::SYSTEM);

    Kernel::init_serial(SERIAL_PORT);
    Kernel::init_logging();
    Kernel::CPU::init_isr();
    Kernel::CPU::init_gdt();
    Kernel::CPU::init_tss();
    Kernel::read_rtc();
    Kernel::init_timer(1000);
    Kernel::init_keyboard(false, "/");
#ifdef GRAPHICS
    Kernel::init_mouse();
#endif
    Kernel::init_syscalls();

    u32 location = *((u32*)mbd->mods_addr); // load modules with GRUB

    uint32_t beginning = location;

    Tar tar;

    // parse the tar file
    tar.parse(location);

    for (int z = 0; z < tar.block_count; z++) {
        beginning += 512;
        beginning += tar.blocks[z].size;
    }

    // for graphics
#ifdef GRAPHICS

    Graphic::init_graphics(mbd);

    init_mem(mbd, beginning);

    init_fs();
    init_all_devs();

    for (int z = 0; z < tar.block_count; z++) {
        if (endswith(tar.blocks[z].name, ".o")) {
            create_file(tar.blocks[z].name, "/bin/", tar.blocks[z].contents);
        } else if (endswith(tar.blocks[z].name, ".txt")) {
            create_file(tar.blocks[z].name, "/usr/documents/", tar.blocks[z].contents);
        } else if (endswith(tar.blocks[z].name, ".r")) {
            create_file(tar.blocks[z].name, "/usr/scripts/", tar.blocks[z].contents);
        } else if (endswith(tar.blocks[z].name, ".sfn")) {
            create_file(tar.blocks[z].name, "/usr/fonts/", tar.blocks[z].contents);
        } else {
            create_file(tar.blocks[z].name, "/usr/", tar.blocks[z].contents);
        }
    }

    Graphic::redraw_background_picture(array);

    UIWindow *win = create_window("Test", DEFAULT_BG, DEFAULT_FG, DEFAULT_FONT);

    UILabel *label = create_label(win, "Test label!", 4, 4);
    draw_widget(label);

    UIButton *button = create_button(win, "Button", 10, 10, callback);
    draw_widget(button);

    draw_window(win);

    Graphic::blit_changes();

    while (true) {}

#else

    init_mem(mbd, beginning);

    init_fs();

    init_vga();
    init_all_devs();

    for (int z = 0; z < tar.block_count; z++) {
        if (endswith(tar.blocks[z].name, ".o")) {
            create_file(tar.blocks[z].name, "/bin/", tar.blocks[z].contents);
        } else if (endswith(tar.blocks[z].name, ".txt")) {
            create_file(tar.blocks[z].name, "/usr/documents/", tar.blocks[z].contents);
        } else if (endswith(tar.blocks[z].name, ".r")) {
            create_file(tar.blocks[z].name, "/usr/scripts/", tar.blocks[z].contents);
        } else if (endswith(tar.blocks[z].name, ".sfn")) {
            create_file(tar.blocks[z].name, "/usr/fonts/", tar.blocks[z].contents);
        } else {
            create_file(tar.blocks[z].name, "/usr/", tar.blocks[z].contents);
        }
    }

    Kernel::scan_buses();

    time_t time = get_time();

    uint16_t *bytes;
    bytes = DiskDrivers::ATA::ata_init(bytes);

    printf("%s Shell\n", System::SYSTEM);

    printf("%s, %s %d\n", weekdays[time.wd-1], months[time.m-1], time.d);

    if (time.min < 10)
        printf("%d:0%d %s\n", time.h, time.min, (char *)(time.pm ? "PM" : "AM"));
    else
        printf("%d:%d %s\n", time.h, time.min, (char *)(time.pm ? "PM" : "AM"));

    uint8_t addr[] = {
        10, 0, 0, 199
    };

    Net::rtl8139 *rtl = new Net::rtl8139();
    rtl->start();
    Net::ARP::send_request(addr);


    //Kernel::CPU::init_tasking();

    //PANIC("s");

    init_rbfs();

    test_install();

    switch_to_user_mode();
    shell();

    Kernel::serial_write_string("\n");
    while (true) {}

#endif
}
