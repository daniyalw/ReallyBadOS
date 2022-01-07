#include <ata.h>

void ata_read(uint8_t *target_address, uint32_t LBA, uint8_t sector_count) {
    while (inb(0x1F7) & STATUS_BSY)
        ;
    outb(0x1F1, 0x00);
    outb(0x1F2, sector_count);
    outb(0x1F3, (uint8_t)LBA);
    outb(0x1F4, (uint8_t)(LBA >> 8));
    outb(0x1F5, (uint8_t)(LBA >> 16));
    outb(0x1F6, 0xE0 | ((LBA >> 24) & 0x0F));
    outb(0x1F7, 0x20);

    uint8_t *target = target_address;

    for (int j = 0; j < sector_count; j++) {
        while (inb(0x1F7) & STATUS_BSY)
            ;
        while (!(inb(0x1F7) & STATUS_RDY))
            ;
        for (int i = 0; i < 256; i++) {
            uint16_t tmpword = inw(0x1F0);
            target[i * 2] = tmpword;
            target[(i * 2) + 1] = tmpword >> 8;
        }

        target += 256;
    }
}

void ata_write_one(uint32_t LBA, uint8_t *bytes) {
    while (inb(0x1F7) & STATUS_BSY)
        ;
    outb(0x1F1, 0x00);
    outb(0x1F2, 0x01);
    outb(0x1F3, (uint8_t)LBA);
    outb(0x1F4, (uint8_t)(LBA >> 8));
    outb(0x1F5, (uint8_t)(LBA >> 16));
    outb(0x1F6, 0xE0 | ((LBA >> 24) & 0x0F));
    outb(0x1F7, 0x30);

    for (int j = 0; j < 1; j++) {
        while (inb(0x1F7) & STATUS_BSY)
            ;
        while (!(inb(0x1F7) & STATUS_RDY))
            ;
        for (int i = 0; i < 256; i++) {
            uint32_t *re = (uint32_t *)*(uint32_t *)&bytes;
            outl(0x1F0, re[i]);
        }
    }
}

// Calculate sectors needed by a file: "(file size in bytes / bytes by sector) +
// 1"
void ata_write(uint32_t LBA, uint8_t sector_count, uint8_t *bytes) {
    for (int i = 0; i < sector_count; i++) {
        ata_write_one(LBA + i, bytes);
        bytes += 512;
    }
}
