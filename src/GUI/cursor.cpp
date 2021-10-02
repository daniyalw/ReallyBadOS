#include "cursor.h"

Cursor::Cursor(int __x, int __y)
{
    _x = __x;
    _y = __y;
}

void Cursor::show()
{
    putpixel(_x, _y, color);
    putpixel(_x, _y+1, color);
    putpixel(_x, _y+2, color);
    putpixel(_x+1, _y+1, color);
    putpixel(_x+2, _y+2, color);
    putpixel(_x+3, _y+3, color);
    putpixel(_x+1, _y, color);
    putpixel(_x+2, _y, color);
}

void Cursor::hide()
{
    putpixel(_x, _y, light_blue);
    putpixel(_x, _y+1, light_blue);
    putpixel(_x, _y+2, light_blue);
    putpixel(_x+1, _y+1, light_blue);
    putpixel(_x+2, _y+2, light_blue);
    putpixel(_x+3, _y+3, light_blue);
    putpixel(_x+1, _y, light_blue);
    putpixel(_x+2, _y, light_blue);
}
