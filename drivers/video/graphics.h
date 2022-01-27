#pragma once

void * framebuffer_addr;
unsigned int pitch;
signed int width;
unsigned int height;
unsigned int bpp;

bool blit_right_away = true;

namespace Graphic {

inline void SetPixel(int x, int y, int color);
inline void blit_changes();
inline void redraw_background_picture(int list[]);
void redraw();

// rect
inline void draw_rect(int x, int y, int w, int h, int c);
inline void draw_rect(int x, int y, int w, int h, int *arr);
inline void draw_rect_arr(int x, int y, int w, int h, int *arr);
void draw_empty_rect(int x, int y, int w, int h, int c);

void clear_screen();

// line
inline void line_low(int x0, int y0, int x1, int y1, int color);
inline void line_high(int x0, int y0, int x1, int y1, int color);
inline void draw_line(int x0, int y0, int x1, int y1, int color);

// text
void center_text_graphics(char * string, int x, int y, int w, int color);

// circle
void topleft_plotpoints(int x, int y, int cx, int cy, int color);
void topright_plotpoints(int x, int y, int cx, int cy, int color);
void bottomright_plotpoints(int x, int y, int cx, int cy, int color);
void bottomleft_plotpoints(int x, int y, int cx, int cy, int color);
void full_plotpoints(int x, int y, int cx, int cy, int color);
void topleft_circle(int cx, int cy, int r, int color);
void topright_circle(int cx, int cy, int r, int color);
void bottomleft_circle(int cx, int cy, int r, int color);
void bottomright_circle(int cx, int cy, int r, int color);
void fill_topleft_corner(int cx, int cy, int radius, int color, int position);
void circle(int cx, int cy, int r, int color);
void fill_circle(int cx, int cy, int radius, int color);
void rounded_rectangle(int x, int y, int w, int h, int r, int color);
void fill_rounded_rectangle(int x, int y, int w, int h, int r, int color);
void tint(int * arr, int w, int h, int color);

}
