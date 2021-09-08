int i = 0;
int white = 0x0F00;
int blue = 0x1F00;
int green = 0x2a00;
int fg = 0;
int bg = 0x2000;

#include "include/outb.h"
#include "include/outb.cpp"
#include "include/cursor.h"
#include "include/cursor.cpp"
#include "include/string.h"
#include "include/string.cpp"
#include "include/stdio.h"
#include "include/ui.cpp"

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


extern "C" void kmain()
{
    int x;
    clear();
    initialize_ui();
    char * mystring = "This is the GUI!";
    x = (80-len(mystring))/2;
    print(mystring, x, bg);
    i = 22 * 80;
    char * taskbar_menu = "This is the taskbar.";
    x = (80-len(taskbar_menu))/2;
    print(taskbar_menu, i+x, 0x1F00);
}
