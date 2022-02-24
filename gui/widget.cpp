#include <gui/widget.h>
#include <gui/window.h>

auto to_widget_draw_hide(widget_t *_widget)
{
    widget_t widget;
    window_t win = windows[_widget->parent_id];
    widget = win.widgets[_widget->id];

    widget.to_draw = _widget->to_draw;
    win.widgets[_widget->id] = widget;
    windows[win.id] = win;

    return win;
}

auto widget_t::draw()
{
    to_draw = true;
    return to_widget_draw_hide(this);
}

auto widget_t::hide()
{
    to_draw = false;
    return to_widget_draw_hide(this);
}

auto add_widget(auto win, widget_t widget)
{
    win.widgets[win.widget_count] = widget;
    win.widget_count++;
    windows[win.id] = win;

    return win;
}
