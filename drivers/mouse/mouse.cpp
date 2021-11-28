#pragma once
#include "../../stdlib/colors.cpp"

//https://forum.osdev.org/viewtopic.php?t=10247

//Mouse functions
void mouse_wait(unsigned char a_type) //unsigned char
{
  unsigned int _time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

void mouse_write(unsigned char a_write) //unsigned char
{
  //Wait to be able to send a command
  mouse_wait(1);
  //Tell the mouse we are sending a command
  outb(0x64, 0xD4);
  //Wait for the final part
  mouse_wait(1);
  //Finally write
  outb(0x60, a_write);
}

unsigned char mouse_read()
{
  //Get's response from mouse
  mouse_wait(0);
  return inb(0x60);
}

void draw_cursor(int x, int y, bool right_c)
{
    int bx = x;
    int by = y;
    int color;

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
      {
          mouse_x -= 0x100;
      }

      if (mouse_y && (mouse_byte[0] & (1 << 5)))
      {
          mouse_y += 0x100;
      }

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
        if (mouse_x < 20 && mouse_y < 20)
            shutdown_os();

    }

    // right click
    if ((mouse_byte[0] >> 1) & 1) {
        right = true;
    }

    // middle click
    if ((mouse_byte[0] >> 2) & 1) {
        middle = true;
    }
    draw_cursor(mouse_x, mouse_y, right);
    right = false;
    left = false;
    middle = false;
}

void mouse_install()
{
  unsigned char _status;  //unsigned char

  //Enable the auxiliary mouse device
  mouse_wait(1);
  outb(0x64, 0xA8);

  //Enable the interrupts
  mouse_wait(1);
  outb(0x64, 0x20);
  mouse_wait(0);
  _status=(inb(0x60) | 2);
  mouse_wait(1);
  outb(0x64, 0x60);
  mouse_wait(1);
  outb(0x60, _status);

  //Tell the mouse to use default settings
  mouse_write(0xF6);
  mouse_read();  //Acknowledge

  //Enable the mouse
  mouse_write(0xF4);
  mouse_read();  //Acknowledge

  register_interrupt_handler(IRQ12, mouse_handler); // interrupts
}
