#include <gui/window.h>

void Window::add_widget(Widget widget)
{
    widget.set_id(widget_count);
    widget.set_winid(id);

    widgets[widget_count] = widget;
    widget_count++;
}

void Window::draw()
{
    draw_rect(x, y, w, h, bg);

    for (int z = 0; z < this->widget_count; z++)
    {
        // we check for the draw function otherwise it jumps to an invalid instruction
        if (widgets[z].draw)
            widgets[z].draw(widgets[z]);
    }
}
