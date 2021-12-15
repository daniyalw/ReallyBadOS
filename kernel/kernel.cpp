int text_x = 0;
int text_y = 0;
int cx = 0;
int cy = 0;
bool booted = false;
int back_buffer[1024*768]; // back buffer for gui

//#define DEBUG

#include <cpuid.h>
#include "sys/background.cpp"
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

#include "sys/io.cpp"
#include "../stdlib/math.cpp"
#include "sys/power/shutdown.cpp"
#include "sys/power/reboot.cpp"
#include "sys/cpu/info.cpp"
#include "../drivers/mouse/cursor.cpp"
#include "../stdlib/string.cpp"
#include "sys/memory/memory.cpp"
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
#include "../gui/window.cpp"

extern "C" void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("Invalid magic number.");
        return; // stop any more kernel code running since it's not multiboot
    }

    // for graphics
    /*
    Kernel::init_serial(SERIAL_PORT);
    Kernel::system_log("%s: line %d: Entered CeneOS kernel.\n", __FILE__, __LINE__);

    framebuffer_addr = (void*)mbd->framebuffer_addr;
    pitch = mbd->framebuffer_pitch;
    width = (uint32_t)mbd->framebuffer_width;
    height = (uint32_t)mbd->framebuffer_height;
    bpp = mbd->framebuffer_bpp;

    Kernel::init_gdt();
    Kernel::init_isr();
    Kernel::read_rtc();
    Kernel::init_timer(1000);
    Kernel::init_keyboard(false);
    Kernel::init_mem(mbd);
    Kernel::init_mouse();

    g_printf((char *)(are_interrupts_enabled() ? "Interrupts are enabled" : "Interrupts are NOT enabled"), 0, 0);
    //Kernel::init_syscalls();

    //Graphic::redraw_background_picture(array);

    window_t * win = create_window("Hello!");
    draw_window(win);

    while (true);
    */

    Kernel::init_serial(SERIAL_PORT);

    Kernel::system_log("Entered CeneOS kernel.\n");

    Kernel::init_gdt();
    Kernel::init_isr();
    Kernel::read_rtc();
    Kernel::init_timer(1000);
    Kernel::init_keyboard(false);
    Kernel::init_mem(mbd);
    Kernel::init_mouse();
    Kernel::init_syscalls();

    Kernel::serial_write_string("\n");
}
