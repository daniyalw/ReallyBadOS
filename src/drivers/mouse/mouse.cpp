#include "mouse.h"

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

static int cursor[13][9] = {
    {2, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 0, 0, 0, 0, 0, 0},
    {2, 1, 1, 2, 0, 0, 0, 0, 0},
    {2, 1, 1, 1, 2, 0, 0, 0, 0},
    {2, 1, 1, 1, 1, 2, 0, 0, 0},
    {2, 1, 1, 1, 1, 1, 2, 0, 0},
    {2, 1, 1, 1, 1, 1, 1, 2, 0},
    {2, 1, 1, 1, 1, 1, 2, 2, 2},
    {2, 1, 2, 2, 2, 1, 2, 0, 0},
    {2, 2, 0, 0, 0, 2, 1, 2, 0},
    {2, 0, 0, 0, 0, 0, 2, 2, 2},
    {0, 0, 0, 0, 0, 0, 0, 2, 0},
};

void draw_cursor(int x, int y)
{
    for (int yy = 0; yy < 13; yy++)
    {
        for (int xx = 0; xx < 8; xx++)
        {
            if (cursor[yy][xx] == 1)
                SetPixel(x+xx, y+yy, 0xFFFFFF);
            else if (cursor[yy][xx] == 2)
                SetPixel(x+xx, y+yy, 0);
        }
    }
}

static void mouse_handler(registers_t regs)
{
  int r = 0;

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

  if (mouse_x > 1024) {
      mouse_x = 1024;
  } else if (mouse_x < 0) {
      mouse_x = 0;
  }

  if (mouse_y > 768) {
      mouse_y = 768;
  } else if (mouse_y < 0) {
      mouse_y = 0;
  }
draw_rect(oldx, oldy, 20, 20, 0x9999);
draw_cursor(mouse_x, mouse_y);

if (mouse_byte[0] & 1) {
    draw_rect(mouse_x+20, mouse_x+20, 5, 5, 0);
}
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
