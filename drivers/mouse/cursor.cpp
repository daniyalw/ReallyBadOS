#pragma once
#include <mouse/cursor.h>
#include <sys/log/log.h>

namespace Kernel {

void update_hardware_cursor(int cursor_x, int cursor_y) {
	unsigned short cursor_location = cursor_y * 80 + cursor_x;

	Kernel::IO::outb(0x3D4, 0x0F);
	Kernel::IO::outb(0x3D5, (unsigned char)(cursor_location & 0xFF));
	Kernel::IO::outb(0x3D4, 0x0E);
	Kernel::IO::outb(0x3D5, (unsigned char )((cursor_location >> 8) & 0xFF));
}

// we don't need an enable function as it is enabled by default
void disable_hardware_cursor() {
	log::warning("Disabled hardware cursor.\n");
	Kernel::IO::outb(0x3D4, 0x0A);
	Kernel::IO::outb(0x3D5, 0x20);
}

unsigned short get_hardware_cursor_pos() {
	unsigned short position = 0;

	Kernel::IO::outb(0x3D4, 0x0F);
	position |= Kernel::IO::inb(0x3D5);

	Kernel::IO::outb(0x3D4, 0x0E);
	position |= ((unsigned char )Kernel::IO::inb(0x3D5)) << 8;

	return position;
}

void set_hardware_cursor(int cursor_y, int cursor_x) {
	unsigned short cursor_location = cursor_y * 80 + cursor_x;

	Kernel::IO::outb(0x3D4, 0x0F);
	Kernel::IO::outb(0x3D5, (unsigned char)(cursor_location & 0xFF));
	Kernel::IO::outb(0x3D4, 0x0E);
	Kernel::IO::outb(0x3D5, (unsigned char )((cursor_location >> 8) & 0xFF));
}

}
