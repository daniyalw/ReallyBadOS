int text_x = 0;
int text_y = 0;
int cx = 0;
int cy = 0;
bool booted = false;
int back_buffer[1024*768]; // back buffer for gui

#include <cpuid.h>
//#include "sys/background.cpp"
#include "sys/log/log.h"
#include "sys/panic/panic.h"
#include "../stdlib/stdint.h"
#include "sys/time/timer.h"
#include "sys/multiboot.h"
#include "sys/io.cpp"
#include "../stdlib/math.cpp"
#include "sys/time/timer.h"
#include "../stdlib/string.h"
#include "../stdlib/memory.h"
#include "../drivers/video/video.h"
#include "sys/acpi/acpi.cpp"
#include "sys/power/shutdown.cpp"
#include "sys/power/reboot.cpp"
#include "../drivers/mouse/mouse.h"
#include "sys/time/time.h"
#include "sys/cpu/info.cpp"
#include "../drivers/mouse/cursor.cpp"
#include "../drivers/keyboard/keyboard.h"
#include "../stdlib/string.cpp"
#include "../stdlib/memory.cpp"
#include "../drivers/video/video.cpp"
#include "sys/log/log.cpp"
#include "sys/descriptors/gdt.cpp"
#include "sys/interrupts/idt.cpp"
#include "sys/interrupts/isr.cpp"
#include "sys/interrupts/interrupts.cpp"
#include "../drivers/video/graphics.h"
#include "../drivers/sound/sound.cpp"
#include "shell/runcommand.cpp"
#include "../drivers/video/font.cpp"
#include "../drivers/video/graphics.cpp"
#include "../drivers/keyboard/keyboard.cpp"
#include "../drivers/mouse/mouse.cpp"
#include "sys/time/time.cpp"
#include "sys/time/timer.cpp"
#include "../stdlib/colors.cpp"
#include "../stdlib/list.cpp"
#include "sys/panic/panic.cpp"
#include "../drivers/video/bga.cpp"

extern "C" void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("Invalid magic number.");
        return; // stop any more kernel code running since it's not multiboot
    }

    // for graphics
    /*
    framebuffer_addr = (void*)mbd->framebuffer_addr;
    pitch = mbd->framebuffer_pitch;
    width = (uint32_t)mbd->framebuffer_width;
    height = (uint32_t)mbd->framebuffer_height;
    bpp = mbd->ramebuffer_bpp;

    // initialize ACPI
    initAcpi();

    // GDT enable
    init_descriptor_tables();
    // IDT & interrupts enable
    isr_install();
    init_timer(100);
    // mouse & keyboard
    init_keyboard();
    mouse_install();


    while (true);
    */

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

}
