#pragma once

#include <sys/io.h>
#include <video/graphics.h>
#include <video/bga.h>
#include <fs.h>

using namespace Filesystem;
using namespace Ramdisk;

namespace Graphic {

void bga_write_register(uint16_t addr, uint16_t data)
{
    outw(BGA_ADDRESS, addr);
    outw(BGA_DATA, data);
}

uint16_t bga_read_register(uint16_t addr)
{
    outw(BGA_ADDRESS, addr);
    return inw(BGA_DATA);
}

bool bga_available()
{
    return (bga_read_register(BGA_ID) == BGA_LATEST);
}

void bga_set_bank(unsigned short bank_number)
{
    bga_write_register(BGA_BANK, bank_number);
}

int bga_set_resolution(int w, int h, int _bpp)
{
    bga_write_register(BGA_REG_ENABLE, BGA_DISABLED);
    bga_write_register(BGA_REG_XRES, w);
    bga_write_register(BGA_REG_YRES, h);
    bga_write_register(BGA_REG_BPP, _bpp);
    bga_write_register(BGA_REG_ENABLE, BGA_ENABLED | BGA_LINEAR_FRAMEBUFFER);
    if (bga_read_register(BGA_REG_XRES) != w || bga_read_register(BGA_REG_YRES) != h)
    {
        bga_write_register(BGA_REG_ENABLE, BGA_DISABLED);
        bga_write_register(BGA_REG_XRES, width);
        bga_write_register(BGA_REG_YRES, height);
        bga_write_register(BGA_REG_BPP, _bpp);
        bga_write_register(BGA_REG_ENABLE, BGA_ENABLED | BGA_LINEAR_FRAMEBUFFER);
        return 1;
    }

    width = w;
    height = h;
    bpp = _bpp;

    return 0;
}

void init_graphics(auto mbd)
{
    framebuffer_addr = (void*)mbd->framebuffer_addr;
    pitch = mbd->framebuffer_pitch;
    width = (uint32_t)mbd->framebuffer_width;
    height = (uint32_t)mbd->framebuffer_height;
    bpp = mbd->framebuffer_bpp;

    if (bga_available())
    {
        Kernel::system_log("\nBGA available!\n");
    }
    else
    {
        Kernel::system_log("\nBGA NOT available!\n");
    }

    char * out;
    out = std::get(out, "\nPitch: %d\nWidth: %d\nHeight: %d\nBPP: %d\n", pitch, width, height, bpp);
    Kernel::system_log(out);
}

}
