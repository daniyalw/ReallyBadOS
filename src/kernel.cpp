// define variables
char * printed;
int custom_cursor = 0;
int cursor_x;
int cursor_y;
int video_x;
int video_y;
int width = 640;
int height = 200;

// import files
#include "kernel/stdint.h"
#include "kernel/system.h"
#include "kernel/outb.h"
#include "kernel/outb.cpp"
#include "kernel/inb.cpp"
#include "kernel/outw.cpp"
#include "kernel/cursor.cpp"
#include "kernel/shutdown.cpp"
#include "kernel/putpixel.cpp"

#include "include/string.cpp"

// drivers
// very bad code ahead
#include "drivers/video/clear.cpp"
#include "drivers/keyboard.cpp"
#include "drivers/mouse.cpp"


#include "include/printf.cpp"

extern "C" void kmain()
{
	short * loc = (short *)0xb8000;
	// get century
	outb(0x70, 0x09);
	unsigned char h = inb(0x71);
}
