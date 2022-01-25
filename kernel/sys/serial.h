#pragma once

#define SERIAL_PORT 0x3f8

int set_port;

namespace Kernel {

static int empty_serial_transmit(int port);
void output_serial_char(int port, char a);
void output_serial_char(char a);
void init_serial(int port);
static int serial_write(int port, char *buf);
void serial_write_string(char *buf);

}
