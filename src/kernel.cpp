// MOUSE NOT WORKING

int text_x = 0;
int text_y = 0;
int cx = 0;
int cy = 0;
int back_buffer[1024*768];

#include <cpuid.h>
#include "include/stdint.h"
#include "sys/time/timer.h"
#include "sys/multiboot.h"
#include "sys/io.cpp"
#include "sys/time/timer.h"
#include "include/string.h"
#include "include/memory.h"
#include "drivers/video/video.h"
#include "sys/acpi/acpi.cpp"
#include "sys/power/shutdown.cpp"
#include "sys/power/reboot.cpp"
#include "drivers/mouse/mouse.h"
#include "sys/cpu/info.cpp"
#include "drivers/mouse/cursor.cpp"
#include "drivers/keyboard/keyboard.h"
#include "include/string.cpp"
#include "include/memory.cpp"
#include "drivers/video/video.cpp"
#include "sys/descriptors/gdt.cpp"
#include "sys/interrupts/idt.cpp"
#include "sys/interrupts/isr.cpp"
#include "sys/interrupts/interrupts.cpp"
#include "drivers/video/graphics.h"
#include "drivers/sound/sound.cpp"
#include "shell/runcommand.cpp"
#include "drivers/video/font.cpp"
#include "drivers/video/graphics.cpp"
#include "drivers/keyboard/keyboard.cpp"
#include "drivers/mouse/mouse.cpp"
#include "sys/time/timer.cpp"
#include "sys/time/time.cpp"
#include "include/colors.cpp"
#include "fs/fs.cpp"
#include "include/list.cpp"
#include "gui/window.cpp"
#include "gui/canvas.cpp"
#include "gui/label.cpp"

extern "C" void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("Invalid magic number.");
        return; // stop any more kernel code running since it's not multiboot
    }

    // for graphics
    framebuffer_addr = (void*)mbd->framebuffer_addr;
    pitch = mbd->framebuffer_pitch;
    width = (uint32_t)mbd->framebuffer_width;
    height = (uint32_t)mbd->framebuffer_height;
    bpp = mbd->framebuffer_bpp;

    // background
    for (int z = 0; z < height; z++)
        for (int b = 0; b < width; b++)
            SetPixel(b, z, 0x9999);

    // display shutdown button on top left
    create_shutdown_button();

    // initialize ACPI
    initAcpi();

    // GDT enable
    init_descriptor_tables();
    // IDT & interrupts enable
    isr_install();
    // mouse & keyboard
    init_keyboard();
    mouse_install();

    // initialize gui
    init_gui();

    // basic gui
    Window window = Window();
    window.draw();

    Label label = Label(window, "Hello!", 3, 0, rgb(255, 0, 0));
    label.draw();

    // if while loop wasn't here than interrupts would shut off
    while (true);
}
