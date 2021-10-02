#include "desktop.h"

Desktop::Desktop()
{
    cursor.show();
}

void Desktop::taskbar()
{
    // display taskbar
    for (int b = 0; b < height-taskbar_y; b++)
    {
        for (int z = 0; z < width; z++)
            putpixel(z, b+taskbar_y, light_green);
    }
}

void Desktop::bg()
{
    // display background
	for (int b = 0; b < taskbar_y; b++)
	{
		for (int z = 0; z < width; z++)
			putpixel(z, b, light_blue);
	}
}

void Desktop::reset()
{
    cursor.hide();
    taskbar();
    bg();
    cursor.show();
}
