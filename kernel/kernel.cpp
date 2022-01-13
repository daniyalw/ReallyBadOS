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
#include <sys.h>
#include <assert.h>
#include <va_list.h>

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
#include "shell/runcommand.cpp"
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

using namespace Filesystem;
using namespace Ramdisk;

extern "C" void kernel_main(multiboot_info_t *mbd, unsigned int magic, uint stack) {
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        Kernel::system_log("Invalid magic.\n"); // we could use printf, but that's text-mode only
        return; // stop any more kernel code running since it's not multiboot
    }

    create_folder("dev");
    create_folder("usr");

    Kernel::init_serial(SERIAL_PORT);

    Kernel::system_log("Entered CeneOS kernel.\n");

    set_seed(4);

    Kernel::init_isr();
    Kernel::init_gdt();
    uint32_t esp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    Kernel::init_tss(5, 0x10, esp);
    Kernel::init_sound();
    Kernel::read_rtc();
    Kernel::init_timer(1000);
    Kernel::init_keyboard(true, "");
    Kernel::init_mouse();
    Kernel::init_syscalls();

    // for graphics
#ifdef GRAPHICS

    u32 location = *((u32*)mbd->mods_addr);
    parse(location);

    for (int z = 0; z < block_count; z++)
        create_file(blocks[z].name, "usr", blocks[z].contents);

    Graphic::init_graphics(mbd);

    Graphic::redraw_background_picture(array);


    Window win = Window();

    win.set_title("File Manager");

    win.set(50, 50, 300, 300);

    int xx = 52, yy = 52;
    //yy += font_height + 2;
    int copyx = xx, copyy = yy;

    //win.draw();
    /*
    for (int z = 0; z < folder_count; z++)
    {
        char *d = (char *)malloc(sizeof(char) * 2, std::len(folders[z].name) + 1);

        draw_string(xx, yy, white, std::get(d, "%s\n", folders[z].name));
        copyy += font_height + 2;
        copyx += font_width * 4;
        xx = copyx;
        yy = copyy;

        for (int b = 0; b < folders[z].file_count; b++)
        {
            draw_string(xx, yy, white, "/%s/%s\n", folders[z].name, folders[z].files[b].name);
            xx = copyx;
            copyy += font_height + 2;
            yy = copyy;
        }

        copyx = 52;
        xx = copyx;
    }
    */
    for (int z = 0; z < folder_count; z++)
    {
        char * name = folders[z].name;
        Widget label = create_label(name, black, xx, yy);
        copyy += font_height + 2;
        copyx += font_width * 4;
        xx = copyx;
        yy = copyy;

        win.add_widget(label);

        for (int b = 0; b < folders[z].file_count; b++)
        {
            Widget sub = create_label(folders[z].files[b].name, black, xx, yy);
            xx = copyx;
            copyy += font_height + 2;
            yy = copyy;

            win.add_widget(sub);
        }

        copyx = 52;
        xx = copyx;
    }

    Kernel::system_log("Window widget count: %d\n", win.widget_count);

    win.draw();

    //draw_string(100, 100, Graphic::rgb(255, 255, 255), "Framebuffer data:\n\tHeight: %d\n\tWidth: %d\n\tBPP: %d", height, width, bpp);

    //int ret = gui_notification("About", "CeneOS is just another hobby operating system!\n2022");

    Graphic::blit_changes();

    while (true);

#else

    u32 location = *((u32*)mbd->mods_addr);

    parse(location);

    for (int z = 0; z < block_count; z++)
    {
        create_file(blocks[z].name, "usr", blocks[z].contents, blocks[z].size * sizeof(char));
    }

    switch_to_user_mode();

    Kernel::serial_write_string("\n");
    while (true) {}

#endif
}
