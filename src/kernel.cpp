// define variables
int x = 0;
int y = 0;
int white = 0x0F00;
int blue = 0x1F00;
int green = 0x2a00;
int color = 0x0F00;
int panic_color = 0x1F00;
int graphics_mode = 0xa000;
char * printed;

// import files
#include "include/outb.h"
#include "include/outb.cpp"
#include "include/cursor.h"
#include "include/cursor.cpp"
#include "include/stdio.h"
#include "include/system.h"
#include "include/system.cpp"

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void putpixel(int pos_x, int pos_y, unsigned char VGA_COLOR)
{
    unsigned char* location = (unsigned char*)0xA0000 + 320 * pos_y + pos_x;
    *location = VGA_COLOR;
}

extern "C" void kmain()
{
	disable_cursor();
	printf(platform);
	printf(" by ");
	printf(author);
	printf("\n");
	system("echo a");
	panic("Memory allocation error.", "Option 1\nOption 2");

}
