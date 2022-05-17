#include "object.h"

namespace UI {

UI::Coords new_widget_coords(UI::Window *win, int padding) {
    UI::Coords _coords;

    auto coords = win->coords;

    const int mx = coords.w;
    const int my = coords.h;

    int x = 0;
    int y = 0;

    for (int z = 0; z < win->child_count; z++) {
        auto child = win->childs[z];

        int w = child->width();
        int h = child->height();

        if ((y + h) >= my) {
            break;
        }

        if ((x + w + padding) >= mx) {
            y += h;
            y += padding;

            x = 0;
        } else {
            x += w;
            x += padding;
        }
    }

    _coords.x = x;
    _coords.y = y;

    return _coords;
}
}
