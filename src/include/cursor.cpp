#include "cursor.h"
#include "inb.h"
#include "inb.cpp"

void update_cursor()
{
	if (custom_cursor == 0)
	{
		unsigned short cursor_loc = y * 80 + x;

	    // cursor LOW port to vga INDEX register
	    outb(0x3D4, 0x0F);
	    outb(0x3D5, (unsigned char)(cursor_loc&0xFF));
	    // cursor HIGH port to vga INDEX register
	    outb(0x3D4, 0x0E);
	    outb(0x3D5, (unsigned char )((cursor_loc>>8)&0xFF));
	}
}

// we don't need an enable function as it is enabled by default

void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

unsigned short get_cursor_pos()
{
   if (custom_cursor == 0)
   {
	   unsigned short pos = 0;
	   outb(0x3D4, 0x0F);
	   pos |= inb(0x3D5);
	   outb(0x3D4, 0x0E);
	   pos |= ((unsigned char )inb(0x3D5)) << 8;
	   return pos;
   }
}

void set_cursor(int _y, int _x)
{
	if (custom_cursor == 0)
	{
		unsigned short cursor_loc = _y * 80 + _x;

	    // cursor LOW port to vga INDEX register
	    outb(0x3D4, 0x0F);
	    outb(0x3D5, (unsigned char)(cursor_loc&0xFF));
	    // cursor HIGH port to vga INDEX register
	    outb(0x3D4, 0x0E);
	    outb(0x3D5, (unsigned char )((cursor_loc>>8)&0xFF));
	}
	else
	{
		short *vidmem = (short *) 0xb8000;
		char * empty = (char *)' ';
		vidmem[_y*80+_x] = blue | empty[0];
		cursor_x = _x;
		cursor_y = _y;
	}
}
