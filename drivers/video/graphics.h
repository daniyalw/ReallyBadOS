#pragma once

void * framebuffer_addr;
unsigned int pitch;
signed int width;
unsigned int height;
unsigned int bpp;

bool blit_right_away = true;

typedef struct {
    int red;
    int green;
    int blue;
} colors_t;

namespace Graphic {

inline void SetPixel(int x, int y, int color);
inline void blit_changes();
inline void redraw_background_picture(int list[]);
void redraw();

// rect
inline void draw_rect(int x, int y, int w, int h, int c);
inline void draw_rect(int x, int y, int w, int h, int *arr);
inline void draw_rect_arr(int x, int y, int w, int h, int *arr);
inline void draw_empty_rect(int x, int y, int w, int h, int c);

void clear_screen();

// line
inline void line_low(int x0, int y0, int x1, int y1, int color);
inline void line_high(int x0, int y0, int x1, int y1, int color);
inline void draw_line(int x0, int y0, int x1, int y1, int color);

// text
void center_text_graphics(char * string, int x, int y, int w, int color);

// circle
inline void topleft_plotpoints(int x, int y, int cx, int cy, int color);
inline void topright_plotpoints(int x, int y, int cx, int cy, int color);
inline void bottomright_plotpoints(int x, int y, int cx, int cy, int color);
inline void bottomleft_plotpoints(int x, int y, int cx, int cy, int color);
inline void full_plotpoints(int x, int y, int cx, int cy, int color);
inline void topleft_circle(int cx, int cy, int r, int color);
inline void topright_circle(int cx, int cy, int r, int color);
inline void bottomleft_circle(int cx, int cy, int r, int color);
inline void bottomright_circle(int cx, int cy, int r, int color);
inline void fill_topleft_corner(int cx, int cy, int radius, int color, int position);
inline void circle(int cx, int cy, int r, int color);
inline void fill_circle(int cx, int cy, int radius, int color);
inline void rounded_rectangle(int x, int y, int w, int h, int r, int color);
inline void fill_rounded_rectangle(int x, int y, int w, int h, int r, int color);
inline void tint(int * arr, int w, int h, int color);

// colors
inline colors_t get_colors(int color);
inline int mix(int color1, int color2);
inline int get_color(int red, int green, int blue);
inline int rgb(int red, int green, int blue);
}
