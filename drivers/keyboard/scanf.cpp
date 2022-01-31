#include <video/video.h>
#include <memory.h>
#include <keyboard/keyboard.h>

char * scanf()
{
    scanf_on = true;

    int limit = 128;
    char * buffer = (char *)malloc(limit);
    int sz = 0;

    while (true)
    {
        char c = scanf_getch();

        if (sz >= limit)
        {
            limit += 128;
            buffer = (char *)realloc(buffer, limit);
        }

        switch (c)
        {
            case '\n':
                buffer[sz] = 0;
                putchar_with_cursor_move('\n');

                scanf_on = false;

                return buffer;

            case '\b':
                if (sz > 0)
                {
                    sz--;
                    buffer[sz] = 0;
                    putchar('\b', 0x0F00);
                    Kernel::update_hardware_cursor(text_x, text_y);
                }
                break;

            case '\t':
                for (int z = 0; z < 4; z++)
                {
                    buffer[sz] = ' ';
                    sz++;
                    printf("    ");
                }

                break;

            default:
                buffer[sz] = c;
                sz++;
                putchar_with_cursor_move(c);

                break;
        }
    }
}
