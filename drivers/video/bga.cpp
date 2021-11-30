#pragma once

#include "../../kernel/sys/io.cpp"
#include "graphics.h"
#include "bga.h"

namespace Kernel {

/*
skiftOS

Copyright © 2018-2021 N. Van Bossuyt & contributors

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

void write_register(uint16_t address, uint16_t data)
{
    outw(BGA_ADDRESS, address);
    outw(BGA_DATA, data);
}

uint16_t read_register(uint16_t address)
{
    outw(BGA_ADDRESS, address);
    return inw(BGA_DATA);
}

int set_resolution(int w, int h)
{
    Kernel::write_register(BGA_REG_ENABLE, BGA_DISABLED);
    Kernel::write_register(BGA_REG_XRES, w);
    Kernel::write_register(BGA_REG_YRES, h);
    Kernel::write_register(BGA_REG_BPP, 32);
    Kernel::write_register(BGA_REG_ENABLE, BGA_ENABLED | BGA_LINEAR_FRAMEBUFFER);
    if (Kernel::read_register(BGA_REG_XRES) != w || Kernel::read_register(BGA_REG_YRES) != h)
    {
        Kernel::write_register(BGA_REG_ENABLE, BGA_DISABLED);
        Kernel::write_register(BGA_REG_XRES, width);
        Kernel::write_register(BGA_REG_YRES, height);
        Kernel::write_register(BGA_REG_BPP, 32);
        Kernel::write_register(BGA_REG_ENABLE, BGA_ENABLED | BGA_LINEAR_FRAMEBUFFER);
        return 1;
    }

    width = w;
    height = h;

    return 0;
}

}
