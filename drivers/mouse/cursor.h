#pragma once

namespace Kernel {

void set_hardware_cursor(int cursor_y, int cursor_x);
void update_hardware_cursor(int cursor_x, int cursor_y);
void disable_hardware_cursor();
unsigned short get_hardware_cursor_pos();

}
