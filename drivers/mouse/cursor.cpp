#pragma once
#include "cursor.h"
#include "../../kernel/sys/log/log.cpp"

void update_hardware_cursor(int cursor_x, int cursor_y)
{
	unsigned short cursor_location = cursor_y * 80 + cursor_x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char)(cursor_location & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char )((cursor_location >> 8) & 0xFF));
}

// we don't need an enable function as it is enabled by default
void disable_hardware_cursor()
{
	system_log("Disabled hardware cursor.\n");
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

unsigned short get_hardware_cursor_pos()
{
	unsigned short position = 0;

	outb(0x3D4, 0x0F);
	position |= inb(0x3D5);

	outb(0x3D4, 0x0E);
	position |= ((unsigned char )inb(0x3D5)) << 8;

	return position;
}

void set_hardware_cursor(int _y, int _x)
{
	unsigned short cursor_location = _y * 80 + _x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char)(cursor_location & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char )((cursor_location >> 8) &0 xFF));

}
