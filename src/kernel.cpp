// define variables
char * printed;
int custom_cursor = 0;
int cursor_x;
int cursor_y;
int video_x;
int video_y;
int width = 640;
int height = 200;

// import files
#include "kernel/stdint.h"
#include "kernel/system.h"
#include "kernel/outb.cpp"
#include "kernel/inb.cpp"
#include "kernel/outw.cpp"
#include "kernel/memory.cpp"
//#include "kernel/cursor.cpp"
#include "kernel/shutdown.cpp"
//#include "kernel/time.cpp"
//#include "kernel/rtc.cpp"

//#include "kernel/putpixel.cpp"

#include "include/string.cpp"

// drivers
// very bad code ahead
//#include "drivers/video/clear.cpp"
// uncomment next include lines if you need them; otherwise os will not compile

#include "drivers/keyboard.cpp"
/*
#include "drivers/mouse.cpp"

#include "include/printf.cpp"
*/

extern "C" void kmain()
{

	unsigned char k;
	string letter = string("");
	short * vidmem = (short *)0xb8000;
	int color = 0x0F00;
	int x = 0;
	int y = 0;
	string last = string("");
	string buffer = string("");
	int b = 0;
	int err = 0;
	char * error;
	char * buffsize;

	vidmem[0] = color | '>';
	x++;

	while (true)
	{
		k = scanf();
		if (k != 0)
		{
			get_key(k);
			letter.set(returned);
			if (returned == "\n")
			{
				y++;
				x = 0;
				if (buffer.startswith("shutdown") == 0)
				{
					shutdown();
				}
				else if (buffer.startswith("echo") == 0)
				{
					for (int z = 4; z < len(buffer.get()); z++)
					{
						vidmem[y*80+x] = color | buffer.get()[z-4];
						x++;
					}
					y++;
					vidmem[y*80+x] = color | '>';
					x++;
				}
				else if (buffer.get() == "")
				{
					y++;
					x = 0;
					vidmem[y*80+x] = color | '>';
					x++;
				}
				else
				{
					y++;
					x = 0;
					for (int z = 0; z < len("Error"); z++)
					{
						vidmem[x+y*80] = color | "Error"[z];
						x++;
					}
					b = 0;
					y++;
					x = 0;
					vidmem[y*80+x] = color | '>';
					x++;
					err++;

				}
				buffer.set("");
			}
			else if (returned != "")
			{
				vidmem[y*80+x] = color | letter.get()[0];
				x++;
				if (x > 80)
				{
					x = 0;
					y++;
				}
				buffer.append(returned);
			}
		}
	}

}
