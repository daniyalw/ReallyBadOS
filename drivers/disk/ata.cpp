#include <disk/ata.h>
#include <string.h>
#include "disk.h"

namespace DiskDrivers {

namespace ATA {

// target_address = your variable to where to store
// lba = which sector
// sector_count = how many sectors to read
uint8_t *ata_read(uint8_t *target_address, uint32_t LBA, uint8_t sector_count) {
    while (Kernel::IO::inb(0x1F7) & STATUS_BSY)
        ;
    Kernel::IO::outb(0x1F1, 0x00);
    Kernel::IO::outb(0x1F2, sector_count);
    Kernel::IO::outb(0x1F3, (uint8_t)LBA);
    Kernel::IO::outb(0x1F4, (uint8_t)(LBA >> 8));
    Kernel::IO::outb(0x1F5, (uint8_t)(LBA >> 16));
    Kernel::IO::outb(0x1F6, 0xE0 | ((LBA >> 24) & 0x0F));
    Kernel::IO::outb(0x1F7, 0x20);

    uint8_t *target = target_address;

    for (int j = 0; j < sector_count; j++) {
        while (Kernel::IO::inb(0x1F7) & STATUS_BSY)
            ;
        while (!(Kernel::IO::inb(0x1F7) & STATUS_RDY))
            ;
        for (int i = 0; i < 256; i++) {
            uint16_t tmpword = Kernel::IO::inw(0x1F0);
            target[i * 2] = tmpword;
            target[(i * 2) + 1] = tmpword >> 8;
        }

        target += 256;
    }

    return target_address;
}

void ata_write_one(uint32_t LBA, uint8_t *bytes) {
    while (Kernel::IO::inb(0x1F7) & STATUS_BSY)
        ;
    Kernel::IO::outb(0x1F1, 0x00);
    Kernel::IO::outb(0x1F2, 0x01);
    Kernel::IO::outb(0x1F3, (uint8_t)LBA);
    Kernel::IO::outb(0x1F4, (uint8_t)(LBA >> 8));
    Kernel::IO::outb(0x1F5, (uint8_t)(LBA >> 16));
    Kernel::IO::outb(0x1F6, 0xE0 | ((LBA >> 24) & 0x0F));
    Kernel::IO::outb(0x1F7, 0x30);

    for (int j = 0; j < 1; j++) {
        while (Kernel::IO::inb(0x1F7) & STATUS_BSY)
            ;
        while (!(Kernel::IO::inb(0x1F7) & STATUS_RDY))
            ;
        for (int i = 0; i < 256; i++) {
            uint32_t *re = (uint32_t *)*(uint32_t *)&bytes;
            Kernel::IO::outl(0x1F0, re[i]);
        }
    }
}

void ata_irq_handler(registers_t *regs) {
    // just so we don't get an unknown interrupt error
}

// Calculate sectors needed by a file: "(file size in bytes / bytes by sector) +
// 1"
void ata_write(uint32_t LBA, uint8_t sector_count, uint8_t *bytes) {
    for (int i = 0; i < sector_count; i++) {
        ata_write_one(LBA + i, bytes);
        bytes += SECTOR_SIZE;
    }
}

int fs_ata_write(fs_node_t *node, int offset, int size, char * str) {
    if (size < 512)
        ata_write_one(offset, (uint8_t *)str);
    else
        ata_write(offset, size/512 + 1, (uint8_t *)str);

    return 0;
}

int fs_ata_read(fs_node_t *node, int offset, int size, char * num) {
    char *data = (char *)malloc(size);
    uint8_t *bytes;

    if (size < 512)
        ata_read(bytes, offset, 1);
    else
        ata_read(bytes, offset, size/512 + 1);

    for (int z = 0; z < size; z++)
        data[z] = (char)bytes[z];

    return 0;
}

void select_drive(uint8_t drive) {
    Kernel::IO::outb(0x1F6, drive);
}

char *model(char *str, uint16_t *id_data) {
    for (int i = 0; i < 40; i += 2) {
        str[i] = id_data[OFFSET_MODEL + i + 1];
        str[i + 1] = id_data[OFFSET_MODEL + i];
    }

    for (int i = 39; i >= 0; i--) {
        if (!isspace(str[i]))
            break;

        str[i] = NULL;
    }

    return str;
}

uint16_t * ata_send_identify(uint16_t *bytes) {
    // check the kind of drive
    uint8_t abc = Kernel::IO::inb(0x1F5);
    bool non_packet = false;

    if (abc == 0xEB) {
        non_packet = true;
    }

    if (!non_packet) {
        select_drive(DRIVE_MASTER);
        Kernel::IO::outb(0x1F2, 0);
        Kernel::IO::outb(0x1F3, 0);
        Kernel::IO::outb(0x1F4, 0);
        Kernel::IO::outb(0x1F5, 0);
        Kernel::IO::outb(0x1F7, 0xEC);
        uint8_t res = Kernel::IO::inb(0x1F7);

        if (res == 0) {
            log::error("Drive does not exist!\n");
            return bytes;
        }

        while (Kernel::IO::inb(0x1F7) & STATUS_BSY)
            ;
        while (!(Kernel::IO::inb(0x1F7) & STATUS_RDY))
            ;

        for (int i = 0; i < 256; i++) {
            bytes[i] = Kernel::IO::inw(0x1F0);
        }

        return bytes;
    } else {
        Kernel::IO::outb(0x1F6, 0xA1);
        Kernel::IO::outb(0x1F2, 0);
        Kernel::IO::outb(0x1F3, 0);
        Kernel::IO::outb(0x1F4, 0);
        Kernel::IO::outb(0x1F5, 0);
        Kernel::IO::outb(0x1F7, 0xEC);
        uint8_t res = Kernel::IO::inb(0x1F7);

        if (res == 0) {
            log::error("Non-existent drive!\n");
            bytes[0] = -1;
            return bytes;
        }

        while (Kernel::IO::inb(0x1F7) & STATUS_BSY)
            ;
        while (!(Kernel::IO::inb(0x1F7) & STATUS_RDY))
            ;

        for (int i = 0; i < 256; i++) {
            bytes[i] = Kernel::IO::inw(0x1F0);
        }

        return bytes;
    }
}

uint32_t total_sectors() {
    uint16_t *bytes;
    uint32_t sectors;

    bytes = ata_send_identify(bytes);
    sectors = (uint32_t)bytes[60];

    return sectors;
}

uint32_t total_bytes() {
    return total_sectors() * 512;
}

uint16_t *ata_init(uint16_t *bytes) {
    Kernel::CPU::register_interrupt_handler(32 + 14, ata_irq_handler);

    bytes = ata_send_identify(bytes);

    if (bytes[0] != -1) {
        Disk *disk = new Disk();

        disk->total_sectors = total_sectors;
        disk->_read = ata_read;
        disk->_write_one = ata_write_one;
        disk->_write = ata_write;

        load_disk(disk);
    } else {
        log::error("ata: bytes are null");
    }

    return bytes;
}

}

}
