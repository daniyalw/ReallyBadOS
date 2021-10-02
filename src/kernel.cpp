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
int custom_cursor = 0;
int cursor_x;
int cursor_y;
int _win = 0;
int width = 320;
int height = 200;
int taskbar_y = 180;
unsigned char light_green = 50;
unsigned char light_blue = 3;

// import files
#include "include/outb.h"
#include "include/outb.cpp"
#include "include/putpixel.cpp"
#include "include/inb.h"
#include "include/inb.cpp"
//#include "include/cursor.h"
//#include "include/cursor.cpp"
//#include "include/stdio.h"
//#include "include/system.h"
//#include "include/system.cpp"
#include "GUI/gui.h"

enum vga_color {
	BLACK = 0,
	ORANGE = 65,
	LIGHT_GREEN = 50,
	LIGHT_BLUE = 100,
	WHITE = 255
};

extern "C" void kmain()
{

	Desktop desktop = Desktop();
	desktop.cursor._x = 100;
	desktop.cursor._y = 150;
	desktop.reset();

	Window win = Window(100, 100);
	Widget widget = Widget(win, 5, 5);
	win.draw();
	widget.draw();

}
