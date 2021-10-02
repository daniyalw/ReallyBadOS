class Widget
{
    int _x;
    int _y;
    int h;
    int l;
    Window window = Window(0, 0);
    int color = 65;
public:
    Widget(Window mywin, int height, int length);
    void draw();
    void hide();
};
