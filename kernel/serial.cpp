#pragma once

#include <kernel/serial.h>

static int is_empty_transmit()
{
	return inb(PORT + 5) & 1;
}

void output_char_serial(char a)
{
	while (is_empty_transmit() == 0);

    // just the PORT is the data register
	outb(PORT, a);
}

void serial_write_string(char *buf)
{
	for (int i = 0; i < len(buf); ++i)
		output_char_serial(buf[i]);
}

void init_serial()
{
	outb(PORT + 1, 0x00); // setting the interrupts register to 0
	outb(PORT + 3, 0x80);
	outb(PORT + 0, 0x03);
	outb(PORT + 1, 0x00);
	outb(PORT + 3, 0x03);
	outb(PORT + 2, 0xC7);
	outb(PORT + 4, 0x0B);
}
