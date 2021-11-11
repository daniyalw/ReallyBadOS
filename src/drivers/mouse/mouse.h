#include "../../include/colors.cpp"

typedef struct {
    bool left_click;
    bool middle_click;
    bool right_click;
    int moved_x;
    int moved_y;
    int x;
    int y;
} mouse_event;


unsigned char mouse_cycle=0;     //unsigned char
unsigned char mouse_byte[3];    //signed char
int mouse_x=0;         //signed char
int mouse_y=0;         //signed char
int oldx = 0;
int oldy = 0;
bool right_clicked = false;
bool left_clicked = false;
bool middle_clicked = false;

void mouse_install();
int mouse_handler();
unsigned char mouse_read();
void draw_cursor(int x, int y, bool right_c);
void mouse_write(unsigned char a_write);
void mouse_wait(unsigned char a_type);

int cursor_height = 19;
int cursor_width = 12;
int cursor_inner = 0;
int cursor_outer = 0xffffff;
int cursor_map[19][12] =
{
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0},
    {2, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0},
    {2, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0},
    {2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0},
    {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
    {2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2},
    {2, 1, 1, 1, 2, 1, 1, 2, 0, 0, 0, 0},
    {2, 1, 1, 2, 0, 2, 1, 1, 2, 0, 0, 0},
    {2, 1, 2, 0, 0, 2, 1, 1, 2, 0, 0, 0},
    {2, 2, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0},
    {0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0}
};
