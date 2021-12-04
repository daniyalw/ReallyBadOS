#pragma once

void * framebuffer_addr;
unsigned int pitch;
signed int width;
unsigned int height;
unsigned int bpp;

void SetPixel(uint32_t x, uint32_t y, int color);
void display_menu(int mx, int my, char * option);
void show_buffer();
void draw_line_up(int x, int y, int l, int c);
void draw_line_down(int x, int y, int l, int c);
void draw_line_right(int x, int y, int l, int c);
void draw_line_left(int x, int y, int l, int c);
void draw_rect(int x, int y, int w, int h, int c);
void clear_screen();
int get_color(int red, int green, int blue);
int rgb(int red, int green, int blue);
void SetPixel(int x, int y, int color);
void draw_empty_rect(int x, int y, int w, int h, int c);
void rounded_rectangle(int x, int y, int w, int h, int r, int color);
void fill_circle(int cx, int cy, int radius, int color);
void circle(int cx, int cy, int r, int color);
void plotpoints(int x, int y, int cx, int cy, int color);
void blit_changes();
