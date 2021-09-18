int x = 0;
int y = 0;
int white = 0x0F00;
int blue = 0x1F00;
int green = 0x2a00;
int color = 0x0F00;
char * printed;

#include "include/outb.h"
#include "include/outb.cpp"
#include "include/cursor.h"
#include "include/cursor.cpp"
#include "include/string.h"
#include "include/string.cpp"
#include "include/stdio.h"

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

// DOESN'T WORK
void INTPutpixel(int x, int y, unsigned char Col) {
	int _CX, _DX;
	unsigned char _AH, _BX, _AL;
    _AH = 0x0C;
    _AL = Col;
    _CX = x;
    _DX = y;
    _BX = 0x01;
    outb(0x10, 0x10);
  }

void putpixel(int pos_x, int pos_y, unsigned char VGA_COLOR) // still doesn't work
{
    unsigned char* location = (unsigned char*)0x0A0000 + 320 * pos_y + pos_x;
    *location = VGA_COLOR;
}

extern "C" void kmain()
{
	disable_cursor();
	//INTPutpixel(100, 100, 255);
	printf("CeneOS by Daniyal Warraich");
}
