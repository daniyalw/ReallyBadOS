#pragma once
#include "cursor.h"

void update_cursor(int cursor_x, int cursor_y)
{
	unsigned short cursor_loc = cursor_y * 80 + cursor_x;

    // cursor LOW port to vga INDEX register
	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char)(cursor_loc&0xFF));
	// cursor HIGH port to vga INDEX register
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char )((cursor_loc>>8)&0xFF));
}

// we don't need an enable function as it is enabled by default

void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

unsigned short get_cursor_pos()
{
	unsigned short pos = 0;
	outb(0x3D4, 0x0F);
	pos |= inb(0x3D5);
	outb(0x3D4, 0x0E);
	pos |= ((unsigned char )inb(0x3D5)) << 8;

	return pos;
}

void set_cursor(int _y, int _x)
{
	unsigned short cursor_loc = _y * 80 + _x;
	// cursor LOW port to vga INDEX register
	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char)(cursor_loc&0xFF));
	// cursor HIGH port to vga INDEX register
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char )((cursor_loc>>8)&0xFF));

}
