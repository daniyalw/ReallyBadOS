/*
 Current bugs:
    - General Protection Fault thrown after some time every time
 */

int text_x = 0;
int text_y = 0;
int cx = 0;
int cy = 0;

#include <cpuid.h>
#include "include/stdint.h"
#include "sys/multiboot.h"
#include "sys/io.cpp"
#include "drivers/mouse/cursor.cpp"
#include "drivers/keyboard/keyboard.h"
#include "include/string.h"
#include "include/string.cpp"
#include "include/memory.cpp"
#include "drivers/video/video.h"
#include "drivers/video/video.cpp"
#include "sys/descriptors/gdt.cpp"
#include "sys/interrupts/idt.cpp"
#include "sys/interrupts/isr.cpp"
#include "sys/interrupts/interrupts.cpp"
#include "drivers/video/graphics.h"
#include "boot/boot.cpp"
#include "sys/cpu/info.cpp"
#include "sys/shutdown/shutdown.cpp"
#include "drivers/video/graphics.cpp"
#include "drivers/sound/sound.cpp"
#include "shell/rc.cpp"
#include "drivers/keyboard/keyboard.cpp"
#include "drivers/mouse/mouse.cpp"
#include "sys/timer.cpp"
#include "sys/time.cpp"

extern "C" void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {

        return; // stop any more kernel code running since it's not multiboot
    }

    // for graphics
    /*
    framebuffer_addr = (void*)mbd->framebuffer_addr;
    pitch = mbd->framebuffer_pitch;
    width = (uint32_t)mbd->framebuffer_width;
    height = (uint32_t)mbd->framebuffer_height;
    bpp = mbd->framebuffer_bpp;
    */

    printf_centered("Terminal", 0);
    printf("\n");
    printf("> ");
    init_descriptor_tables();
    isr_install();
    init_keyboard();
}
