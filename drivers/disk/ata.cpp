#include <disk/ata.h>
#include <string.h>
#include "disk.h"

using namespace std;

// target_address = your variable to where to store
// lba = which sector
// sector_count = how many sectors to read
uint8_t *ata_read(uint8_t *target_address, uint32_t LBA, uint8_t sector_count) {
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

    return target_address;
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

void ata_irq_handler(registers_t *regs)
{
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

int fs_ata_write(fs_node_t *node, int offset, int size, char * str)
{
    if (size < 512)
        ata_write_one(offset, (uint8_t *)str);
    else
        ata_write(offset, size/512 + 1, (uint8_t *)str);

    return 0;
}

int fs_ata_read(fs_node_t *node, int offset, int size, char * num)
{
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

void select_drive(uint8_t drive)
{
    outb(0x1F6, drive);
}

char *model(char *str, uint16_t *id_data)
{
    for (int i = 0; i < 40; i += 2)
    {
        str[i] = id_data[OFFSET_MODEL + i + 1];
        str[i + 1] = id_data[OFFSET_MODEL + i];
    }

    for (int i = 39; i >= 0; i--)
    {
        if (!isspace(str[i]))
            break;

        str[i] = NULL;
    }

    return str;
}

uint16_t * ata_send_identify(uint16_t *bytes)
{
    // check the kind of drive
    uint8_t abc = inb(0x1F5);
    bool non_packet = false;

    if (abc == 0xEB) {
        non_packet = true;
    }

    if (!non_packet) {
        select_drive(DRIVE_MASTER);
        outb(0x1F2, 0);
        outb(0x1F3, 0);
        outb(0x1F4, 0);
        outb(0x1F5, 0);
        outb(0x1F7, 0xEC);
        uint8_t res = inb(0x1F7);

        if (res == 0) {
            log::error("Drive does not exist!\n");
            return bytes;
        }

        while (inb(0x1F7) & STATUS_BSY)
            ;
        while (!(inb(0x1F7) & STATUS_RDY))
            ;

        for (int i = 0; i < 256; i++) {
            bytes[i] = inw(0x1F0);
        }

        return bytes;
    } else {
        outb(0x1F6, 0xA1);
        outb(0x1F2, 0);
        outb(0x1F3, 0);
        outb(0x1F4, 0);
        outb(0x1F5, 0);
        outb(0x1F7, 0xEC);
        uint8_t res = inb(0x1F7);

        if (res == 0) {
            log::error("Non-existent drive!\n");
            bytes[0] = -1;
            return bytes;
        }

        while (inb(0x1F7) & STATUS_BSY)
            ;
        while (!(inb(0x1F7) & STATUS_RDY))
            ;

        for (int i = 0; i < 256; i++) {
            bytes[i] = inw(0x1F0);
        }

        return bytes;
    }
}

uint32_t total_sectors()
{
    uint16_t *bytes;
    uint32_t sectors;

    bytes = ata_init(bytes);
    sectors = (uint32_t)bytes[60];

    return sectors;
}

uint32_t total_bytes()
{
    return total_sectors() * 512;
}

/*
FILE * read_file_from_disk(uint32_t LBA, uint32_t sectors)
{
    FILE * file;
    uint8_t * bytes;

    ata_read(bytes, LBA, sectors);

    file = (FILE *)bytes;

    return file;
}
*/

uint16_t *ata_init(uint16_t *bytes)
{
    Kernel::register_interrupt_handler(32 + 14, ata_irq_handler);

    bytes = ata_send_identify(bytes);

    if (bytes[0] != -1)
    {
        disk_t *disk = (disk_t *)malloc(sizeof(disk_t *));

        disk->total_sectors = total_sectors;
        disk->read = ata_read;
        disk->write = ata_write_one;

        load_disk(disk);
    }
    else
    {
        log::error("ata: bytes are null");
    }

    return bytes;
}
