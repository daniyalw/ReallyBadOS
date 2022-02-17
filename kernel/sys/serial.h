#pragma once

#define SERIAL_PORT 0x3f8

int set_port;

#define FG_BLACK_SERIAL "30"
#define FG_RED_SERIAL "31"
#define FG_GREEN_SERIAL "32"
#define FG_YELLOW_SERIAL "33"
#define FG_BLUE_SERIAL "34"
#define FG_MAGENTA_SERIAL "35"
#define FG_CYAN_SERIAL "36"
#define FG_WHITE_SERIAL "37"
#define FG_GREY_SERIAL "1;30"
#define FG_BR_BLACK_SERIAL FG_GREY_SERIAL
#define FG_BR_RED_SERIAL "1;31"
#define FG_BR_GREEN_SERIAL "1;32"
#define FG_BR_YELLOW_SERIAL "1;33"
#define FG_BR_BLUE_SERIAL "1;34"
#define FG_BR_MAGENTA_SERIAL "1;35"
#define FG_BR_CYAN_SERIAL "1;36"
#define FG_BR_WHITE_SERIAL "1;37"

#define BG_BLACK_SERIAL "40"
#define BG_RED_SERIAL "41"
#define BG_GREEN_SERIAL "42"
#define BG_YELLOW_SERIAL "43"
#define BG_BLUE_SERIAL "44"
#define BG_MAGENTA_SERIAL "45"
#define BG_CYAN_SERIAL "46"
#define BG_WHITE_SERIAL "47"
#define BG_GREY_SERIAL "1;40"
#define BG_BR_BLACK_SERIAL BG_GREY_SERIAL
#define BG_BR_RED_SERIAL "1;41"
#define BG_BR_GREEN_SERIAL "1;42"
#define BG_BR_YELLOW_SERIAL "1;43"
#define BG_BR_BLUE_SERIAL "1;44"
#define BG_BR_MAGENTA_SERIAL "1;45"
#define BG_BR_CYAN_SERIAL "1;46"
#define BG_BR_WHITE_SERIAL "1;47"

namespace Kernel {

static int empty_serial_transmit(int port);
void output_serial_char(int port, char a);
void output_serial_char(char a);
void init_serial(int port);
static int serial_write(int port, char *buf);
void serial_write_string(char *buf, ...);
void serial_write_string(char *buf, va_list va);

}

void p_template(char * color, char *text, char *buf, va_list va);
void p_warning(char * text, ...);
void p_info(char * text, ...);
void p_error(char * text, ...);

char *get_color(char *bg, char *fg);
