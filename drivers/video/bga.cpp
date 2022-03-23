#pragma once

#include <sys/io.h>
#include <video/graphics.h>
#include <video/bga.h>
#include <sys/pci/pci.h>
#include <string.h>

namespace Graphic {

void bga_write_register(uint16_t addr, uint16_t data)
{
    Kernel::IO::outw(BGA_ADDRESS, addr);
    Kernel::IO::outw(BGA_DATA, data);
}

uint16_t bga_read_register(uint16_t addr)
{
    Kernel::IO::outw(BGA_ADDRESS, addr);
    return Kernel::IO::inw(BGA_DATA);
}

bool bga_available()
{
    return (bga_read_register(BGA_ID) == BGA_LATEST);
}

void bga_set_bank(unsigned short bank_number)
{
    bga_write_register(BGA_BANK, bank_number);
}

bool is_bga_device()
{
    Kernel::PCIDevice *device = Kernel::find_device(0x1111, 0x1234);

    if (device == NULL)
        return false;
    else
        return true;
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
        log::info("\nBGA available!\n");
    }
    else
    {
        log::warning("\nBGA NOT available!\n");
    }

    char * out;
    out = get(out, "Pitch: %d\nWidth: %d\nHeight: %d\nBPP: %d", pitch, width, height, bpp);
    log::info(out);
}

}
