#pragma once

class Label
{
    int x, y;
    int w, h;
    int fg, bg;
    Window window;
    int padding;
public:
    char * text;
    Label(Window win, char * t, int _padding, int _fg, int _bg);
    void draw();
    void hide();
    void move(int newx, int newy);
};
