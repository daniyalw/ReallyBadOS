int text_x = 0;
int text_y = 0;
int cx = 0;
int cy = 0;
bool booted = false;
int back_buffer[1024*768]; // back buffer for gui

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
#include "sys/acpi/acpi.cpp"
#include "sys/power/shutdown.cpp"
#include "sys/power/reboot.cpp"
#include "sys/cpu/info.cpp"
#include "../drivers/mouse/cursor.cpp"
#include "../stdlib/string.cpp"
#include "../stdlib/memory.cpp"
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
#include "../gui/gui.cpp"
#include "../drivers/video/saturation.cpp"

extern "C" void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("Invalid magic number.");
        return; // stop any more kernel code running since it's not multiboot
    }

    // for graphics
    /*
    init_serial(SERIAL_PORT);
    Kernel::system_log("Entered kernel.\n");

    framebuffer_addr = (void*)mbd->framebuffer_addr;
    pitch = mbd->framebuffer_pitch;
    width = (uint32_t)mbd->framebuffer_width;
    height = (uint32_t)mbd->framebuffer_height;
    bpp = mbd->framebuffer_bpp;

    // initialize ACPI
    Kernel::initAcpi();

    // GDT enable
    Kernel::init_gdt();
    // IDT & interrupts enable
    Kernel::isr_install();
    //init_timer(1000);
    // mouse & keyboard
    init_keyboard(false);
    mouse_install();

    Graphic::redraw_background_picture(array);

    while (true);
    */

    printf("%s %s\n\n", System::SYSTEM, System::VERSION);

    init_serial(SERIAL_PORT);
    Kernel::system_log("Entered CeneOS kernel.\n");

    // initialize ACPI
    Kernel::initAcpi();

    // GDT enable
    Kernel::init_gdt();
    // IDT & interrupts enable
    Kernel::isr_install();
    init_timer(1000);
    // mouse & keyboard
    init_keyboard(false);
    //mouse_install();

    //printf("%d", kernel_main);
    char * addr;
    std::itoa((int)kernel_main, addr);
    Kernel::system_log("\nkernel_main() address: 0x");
    Kernel::system_log(addr);

    Kernel::system_log("\n");

    init_mem();

    char * buff;
    buff = Kernel::get_log(buff);
    printf(buff);

    sleep(2);

    clear();

    Kernel::set_hardware_cursor(1, 0);

    printf_centered("Terminal", 0);
    printf("\n/> ");
    terminal_on = true; // init terminal

    serial_write_string("\n");

}
