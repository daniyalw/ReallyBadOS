#include "window.h"
#include "object.h"

namespace UI {
    class List : public UI::Object {
    public:
        virtual void event_handle(UI::Object *object, UI::Event *event) override {
            log::info("GUI: list: event");
            if (event->type == EVENT_MOUSE_LEFT) {
                log::info("GUI: list: left");
                int y = coords.y + padding + parent->coords.y;
                int old = selected;
                selected = -1;
                bool done = false;

                if (event->coords.y >= y && event->coords.y <= (y + font_height + padding)) {
                    selected = 0;
                    done = true;
                }

                for (int z = 0; z < line_count; z++) {
                    if (done) {
                        break;
                    }

                    if (z == 10) {
                        // no more than 10 lines
                        // TODO add ability to scroll
                        break;
                    }

                    y += font_height + padding;

                    if (event->coords.y >= y && event->coords.y <= (y + font_height + padding)) {
                        selected = 0;
                        done = true;
                    }
                }

                log::info("GUI: list: selected: %d", selected);

                if (selected >= 0) {
                    auto _coords = parent->coords;


                    Graphic::draw_rect(coords.x + parent->coords.x, y, coords.w, font_height + padding, change);
                    Graphic::draw_empty_rect(coords.x + _coords.x - 1, coords.y + _coords.y - 1, coords.w, coords.h, 0);
                    draw_string(coords.x + padding + parent->coords.x, y + padding, fg, lines[selected + 1]);
                }
            }
        }

        virtual void draw_object(UI::Object *object, UI::Coords _coords) override {
            Graphic::draw_rect(coords.x + _coords.x, coords.y + _coords.y, coords.w, coords.h, bg);
            Graphic::draw_empty_rect(coords.x + _coords.x - 1, coords.y + _coords.y - 1, coords.w, coords.h, 0);

            int y = coords.y + padding + _coords.y;

            for (int z = 0; z < line_count; z++) {
                if (z == 10) {
                    // no more than 10 lines
                    // TODO add ability to scroll
                    break;
                }
                log::info("GUI: Drawing line '%s'", lines[z]);

                draw_string(coords.x + padding + _coords.x, y, fg, lines[z]);
                y += font_height + padding;
            }
        }

        virtual int height() {
            return (font_height + 2) * 10;
        }

        virtual int width() {
            return font_width * 20 + padding + 2;
        }

        void add(char *str, ...) {
            char *out = (char *)malloc(100);
            memset(out, 0, 100);

            va_list va;
            va_start(va, str);
            vsprintf(out, str, va);
            va_end(va);

            lines[line_count] = out;
            line_count++;
        }

        void remove(int c) {
            if (c < 0 || c >= line_count) {
                return;
            }

            free(lines[c]);

            for (int z = c; z < line_count; z++) {
                lines[z] = lines[z + 1];
            }

            line_count--;
        }

        int get_selected() {
            return selected;
        }

        char **lines;
        int line_count = 0;
        int padding = 2;
        int change;
        int selected = -1;
    };

    UI::List *list(UI::Window *win, int x, int y, int bg, int fg, int change);
    UI::List *list(UI::Window *win, int x, int y);
}

void add_str(UI::List *list, char *str, ...);
