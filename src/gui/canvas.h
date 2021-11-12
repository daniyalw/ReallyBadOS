#pragma once

class Canvas
{
    int x, y;
    int w, h;
    Window window;
public:
    Canvas(Window win);
    void draw();
    void hide();
    void move_to(int newx, int newy);
    void putpixel(int _x, int _y, int _color);
};
