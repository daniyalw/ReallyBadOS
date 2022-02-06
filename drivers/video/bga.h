#pragma once

#define BGA_ADDRESS 0x01CE
#define BGA_DATA 0x01CF
#define BGA_LATEST 0xB0C5

#define BGA_ID 0

#define BGA_REG_XRES 0x1
#define BGA_REG_YRES 0x2
#define BGA_REG_BPP 0x3
#define BGA_REG_ENABLE 0x4
#define BGA_BANK 0x5

#define BGA_DISABLED 0x00
#define BGA_ENABLED 0x01
#define BGA_LINEAR_FRAMEBUFFER 0x40

void bga_write_register(uint16_t addr, uint16_t data);
uint16_t bga_read_register(uint16_t addr);
bool bga_available();
void bga_set_bank(unsigned short bank_number);
int bga_set_resolution(int w, int h, int _bpp);
bool is_bga_device();
