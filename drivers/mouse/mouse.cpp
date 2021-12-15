#pragma once
#include <colors.h>
#include <drivers/mouse/mouse.h>
#include "../../gui/window.cpp"

//https://forum.osdev.org/viewtopic.php?t=10247

void mouse_wait(unsigned char at)
{
    unsigned int timeout = 100000;

    switch (at)
    {
        case 0:
            while (timeout--)
                if ((inb(0x64) & 1) == 1)
                    return;

            break;
        default:
            while (timeout--)
                if ((inb(0x64) & 2) == 0)
                    return;

            break;
    }
}

void mouse_write(unsigned char data)
{
    // wait for the mouse to be ready
    mouse_wait(1);
    // tell them mouse we are sending a command
    outb(0x64, 0xD4);
    // wait so the mouse is ready for the command
    mouse_wait(1);
    // the actual write
    outb(0x60, data);
}

unsigned char mouse_read()
{
  mouse_wait(0);
  return inb(0x60);
}

void draw_cursor(int x, int y, bool right_c, bool left, bool middle)
{
    int bx = x;
    int by = y;
    int color;

    if (left && oldx != x && oldy != y)
    {
        window_t * win = get_window_pixel(x, y);
        if (win != NULL)
        {
            if (win->dragging)
            {
                Kernel::system_log("Moving window ID %d to %d x %d y\n", win->id, x, y);
                move_window(win);
            }
            else
            {
                Kernel::system_log("Setting dragging of window ID %d to true\n", win->id);
                win->dragging = true;
                windows.replace(win, win->z);
            }
        }
        else
        {
            Kernel::system_log("win is NULL\n");
        }
    }
    else if (!left)
    {
        for (int z = 0; z < windows.size(); z++)
        {
            window_t * win = windows.get(z);
            win->dragging = false;
            windows.replace(win, win->z);
        }
    }

    for (int z = 0; z < cursor_height; z++)
    {
        for (int b = 0; b < cursor_width; b++)
        {
            color = cursor_map[z][b];
            if (color == 0)
                color = back_buffer[by*1024+bx];
            else if (color == 1)
                color = cursor_inner;
            else if (color == 2)
                color = cursor_outer;
            uint8_t * where = (uint8_t*)(by * pitch + (bx * (bpp/8)) + (uint32_t)framebuffer_addr);
            where[0] = color & 255;
            where[1] = (color >> 8) & 255;
            where[2] = (color >> 16) & 255;
            bx++;
        }
        bx -= 12;
        by++;
    }
}

static void mouse_handler(registers_t regs)
{
    int r = 0;
    int mx = 0;
    int my = 0;
    bool left, right, middle;

    switch(mouse_cycle)
    {
        case 0:
            mouse_byte[0]=mouse_read();
            mouse_cycle++;
            break;
        case 1:
            mouse_byte[1]=mouse_read();
            mouse_cycle++;
            break;
        case 2:
            mouse_byte[2]=mouse_read();
            oldx = mouse_x;
            oldy = mouse_y;
            mouse_x+=(int)mouse_byte[1];
            mouse_y-=(int)mouse_byte[2];

            if (mouse_x && (mouse_byte[0] & (1 << 4)))
                mouse_x -= 0x100;

            if (mouse_y && (mouse_byte[0] & (1 << 5)))
                mouse_y += 0x100;

            mouse_cycle=0;
            r = 1;
            break;
        default:
            return;
    }

    if (mouse_x >= width) {
        mouse_x = width;
    } else if (mouse_x < 0) {
        mouse_x = 0;
    }

    if (mouse_y > height) {
        mouse_y = height;
    } else if (mouse_y < 0) {
        mouse_y = 0;
    }

    int col;
    for (int z = 0; z < cursor_height; z++)
        for (int b = 0; b < cursor_width; b++)
        {
            col = back_buffer[(oldy+z)*width+(oldx+b)];
            uint8_t * where = (uint8_t*)((oldy+z) * pitch + ((oldx+b) * (bpp/8)) + (uint32_t)framebuffer_addr);
            where[0] = col & 255;              // BLUE
            where[1] = (col >> 8) & 255;   // GREEN
            where[2] = (col >> 16) & 255;  // RED
        }



    // left click
    if (mouse_byte[0] & 1) {
        left = true;

        // check if power off button was clicked
        //if (mouse_x < 20 && mouse_y < 20)
        //    Kernel::shutdown_os();

    }

    // right click
    if ((mouse_byte[0] >> 1) & 1) {
        right = true;
    }

    // middle click
    if ((mouse_byte[0] >> 2) & 1) {
        middle = true;
    }
    draw_cursor(mouse_x, mouse_y, right, left, middle);
    right = false;
    left = false;
    middle = false;

    //Graphic::redraw_background_picture(back_buffer);
}

namespace Kernel {

void init_mouse()
{
    unsigned char mstatus;

    mouse_wait(1);
    outb(0x64, 0xA8);

    mouse_wait(1);
    outb(0x64, 0x20);

    mouse_wait(0);
    mstatus = inb(0x60) | 2;
    mouse_wait(1);

    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, mstatus);

    mouse_write(0xF6);
    mouse_read();

    mouse_write(0xF4);
    mouse_read();

    Kernel::register_interrupt_handler(IRQ12, mouse_handler);
}

}
