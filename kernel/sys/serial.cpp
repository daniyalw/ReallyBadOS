#pragma once

#include <sys/serial.h>
#include <sys/io.h>

namespace Kernel {

static int empty_serial_transmit(int port)
{
	return Kernel::IO::inb(port + 5) & 0x20;
}

void output_serial_char(int port, char a)
{
	while (empty_serial_transmit(port) == 0)
		;

	Kernel::IO::outb(port, a);
}

static int serial_write(int port, char *buf)
{
	for (int i = 0; i < strlen(buf); ++i)
		output_serial_char(port, buf[i]);
	return 0;
}

void output_serial_char(char a)
{
	output_serial_char(SERIAL_PORT, a);
}

char *serial_read(char *buf)
{
	UNUSED(buf);
	return NULL;
}

void serial_write_string(char *buf, va_list va)
{
	char *ret = vsprintf("", buf, va);

	serial_write(SERIAL_PORT, ret);
}

void serial_write_string(char *buf, ...)
{
	va_list va;

	va_start(va, buf);
	serial_write_string(buf, va);
	va_end(va);
}

void init_serial(int port)
{
	Kernel::IO::outb(port + 1, 0x00);  // Disable all interrupts
	Kernel::IO::outb(port + 3, 0x80);  // Enable DLAB (set baud rate divisor)
	Kernel::IO::outb(port + 0, 0x03);  // Set divisor to 3 (lo byte) 38400 baud
	Kernel::IO::outb(port + 1, 0x00);  //                  (hi byte)
	Kernel::IO::outb(port + 3, 0x03);  // 8 bits, no parity, one stop bit
	Kernel::IO::outb(port + 2, 0xC7);  // Enable FIFO, clear them, with 14-byte threshold
	Kernel::IO::outb(port + 4, 0x0B);  // IRQs enabled, RTS/DSR set
}

}

char *get_color(char *bg, char *fg)
{
    return get("", "\033[%s;%sm", fg, bg);
}

void p_template(char *color, char *text, char *buf, va_list va)
{
    Kernel::serial_write_string(color); // change the color to whatever is specified

    Kernel::serial_write_string(text);

    Kernel::serial_write_string(REG_SERIAL_OUT); // change the color to regular

    Kernel::serial_write_string(buf, va);
}

void p_warning(char *text, ...)
{
    auto yellow = get_color("40", "1;33");
    va_list va;

    va_start(va, text);
    p_template(yellow, "Warning: ", text, va);
    va_end(va);
}

void p_error(char *text, ...)
{
    auto red = get_color("40", "1;31");
    va_list va;

    va_start(va, text);
    p_template(red, "Error: ", text, va);
    va_end(va);
}

void p_info(char *text, ...)
{
    auto light_blue = get_color("40", "36");
    va_list va;

    va_start(va, text);
    p_template(light_blue, "Info: ", text, va);
    va_end(va);
}
