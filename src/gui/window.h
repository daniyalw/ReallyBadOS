#pragma once

class Window
{
    int x, y;
    int w, h;
    int z;
    bool shown;
    int color = rgb(255, 0, 0);
    int id;
public:
    Window();
    void draw();
    void hide();
    void move(int newx, int newy);
    int getx();
    int gety();
    int getw();
    int geth();
    bool is_active();
    int get_color();
    void putpixel(int _x, int _y, int _color);
    void save_window();
};

Window * windows[128];
int win_count = 0;

void init_gui();
