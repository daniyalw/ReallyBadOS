#include <string.h>
#include <memory.h>
#include <drivers/keyboard/keyboard.h>
#include <sys/log/log.h>
#include "keys.cpp"

void get_key(unsigned char code);

bool keyboard_lock()
{
    if (!keyboard_locked)
    {
        keyboard_locked = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool keyboard_unlock()
{
    if (keyboard_locked)
    {
        keyboard_locked = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool is_keyboard_locked()
{
    return keyboard_locked;
}

int get_buffer_size()
{
    return buffer_size;
}

void edit_buffer_size(int change)
{
    buffer_size += change;
}

static void scan_key(registers_t regs)
{
    current_key = inb(0x60);
    get_key(current_key);

    return;
}

void get_key(unsigned char code)
{
    char key;

    if (code == 0)
    {
        returned = "";
        return;
    }

    int index = -1;

    for (int z = 0; z < keycode_length; z++)
    {
        if (keycodes[z] == code)
        {
            index = z;
            break;
        }
    }

    if (index == -1)
    {
        int i = -1;

        for (int z = 0; z < special_codes_length; z++)
        {
            if (special_codes[z] == code)
            {
                i = z;
                break;
            }
        }

        if (i == -1)
            return;

        if (code == 0x3A || code == 0x2A || code == 0x36)
        {
            if (shifted)
            {
                shifted = false;
                key = 0;
            }
            else
            {
                shifted = true;
                key = 0;
            }
        }
    }
    else
    {
        if (shifted)
            key = letters_upper[index];
        else
            key = letters_lower[index];
    }

    if (key != 0) {
        buffer[buffer_size] = key;
        buffer[buffer_size+1] = 0;
        buffer_size++;
        entered_len++;
        keyboard_unlock();
    }
}

namespace Kernel {

void init_keyboard(bool on, char * cd) {
    for (int z = 0; z < 1000; z++)
        buffer[z] = 0;

    for (int z = 0; z < 128; z++) {
        current_display[z] = 0;
    }

    for (int z = 0; z < std::len(cd); z++)
        current_display[z] = cd[z];
    current_display_len = std::len(cd);

    if (on) {
        printf_centered("Terminal", 0); // 0 indicates line 0
        printf("\n/> ");
    }
    terminal_on = on;
    Kernel::system_log("Enabled keyboard.\n");
    // register the interrupt
    Kernel::register_interrupt_handler(IRQ1, scan_key);
}

}
