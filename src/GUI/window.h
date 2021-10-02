class Window {
    int widget_i_max = 100;
public:
    int _x;
    int _y;
    int free_x = 1;
    int free_y = 1;
    int widget_i = 0;
    int widgets[100];
private:
    int wh = 30; // defaults
	int ww = 30;
    unsigned char color = 150;
    unsigned char top = 10;
public:
    void draw();
    Window(int __x, int __y);
    void hide();
    void move(int __x, int __y);
    void resize(int h, int w);
    void update_free();
};
