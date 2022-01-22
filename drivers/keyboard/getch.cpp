#include <drivers/keyboard/keyboard.h>

char scanf_getch()
{
    scanf_on = true;

    int buffs = buffer_size;
    keyboard_lock();

    while (true)
    {
        if (!is_keyboard_locked())
        {
            char k = buffer[get_buffer_size()-1];
            edit_buffer_size(1);
            return k;
        }
    }
}

char getch()
{
    scanf_on = true;

    int buffs = buffer_size;
    keyboard_lock();

    while (true)
    {
        if (!is_keyboard_locked())
        {
            char k = buffer[get_buffer_size()-1];
            edit_buffer_size(1);

            scanf_on = false;
            return k;
        }
    }
}
