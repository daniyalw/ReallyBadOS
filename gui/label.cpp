#include <gui/widget.h>
#include <drivers/video/video.h>
#include <font.h>

using namespace Graphic;

void draw_label(Widget label)
{
    int offset = 0;

    if (label.bg != BG_TRANSPARENT)
        draw_rect(label.x - label.padding/2, label.y - label.padding/2, label.w + label.padding/2, label.h + label.padding/2, label.bg);
    draw_string((char *)(label.extras[0]), label.x, label.y, label.fg);
}

Widget create_label(Window win, int bg, int fg, int x, int y, int padding, char *text, ...)
{
    Widget widget;
    va_list va;

    va_start(va, text);
    text = vsprintf("", text, va);
    va_end(va);

    widget.set_extra(0, text);

    widget.extras = (void **)malloc(sizeof(void **) * 10);

    widget.setx(x);
    widget.sety(y);
    widget.setw(len(text) * font_width);
    widget.seth(font_height + 2);

    widget.set_draw(draw_label);

    widget.setbg(bg);
    widget.setfg(fg);

    widget.set_padding(padding);

    win.add_widget(widget);

    return widget;
}
