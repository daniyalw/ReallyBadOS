#pragma once

#include <sys/serial.h>
#include <sys/io.h>
#include <fs.h>

namespace Kernel {

static int empty_serial_transmit(int port)
{
	return inb(port + 5) & 0x20;
}

void output_serial_char(int port, char a)
{
	while (empty_serial_transmit(port) == 0)
		;

	outb(port, a);
}

static int serial_write(int port, char *buf)
{
	for (int i = 0; i < len(buf); ++i)
		output_serial_char(port, buf[i]);
	return 0;
}

void output_serial_char(char a)
{
	output_serial_char(SERIAL_PORT, a);
}

void serial_write_string(char *buf)
{
	serial_write(SERIAL_PORT, buf);
}

char *serial_read(char *buf)
{
	UNUSED(buf);
	return NULL;
}

void init_serial(int port)
{
	outb(port + 1, 0x00);  // Disable all interrupts
	outb(port + 3, 0x80);  // Enable DLAB (set baud rate divisor)
	outb(port + 0, 0x03);  // Set divisor to 3 (lo byte) 38400 baud
	outb(port + 1, 0x00);  //                  (hi byte)
	outb(port + 3, 0x03);  // 8 bits, no parity, one stop bit
	outb(port + 2, 0xC7);  // Enable FIFO, clear them, with 14-byte threshold
	outb(port + 4, 0x0B);  // IRQs enabled, RTS/DSR set

	create_file("serial", "dev", serial_read, serial_write_string);
}

}
