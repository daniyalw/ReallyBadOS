#pragma once

#define PORT 0x3f8 // com port 1

int set_port;

static int is_empty_transmit();
void output_char_serial(char a);
void init_serial();
void serial_write_string(char *buf);
