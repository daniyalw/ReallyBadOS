void * framebuffer_addr;
unsigned int pitch;
signed int width;
unsigned int height;
unsigned int bpp;
int cursor_x = 300;
int cursor_y = 300;

void SetPixel(uint32_t x, uint32_t y, int color);
void SetPixel(int x, int y, int color);
void draw_empty_rect(int x, int y, int w, int h, int c);
