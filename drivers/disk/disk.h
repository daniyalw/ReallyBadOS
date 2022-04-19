#pragma once

class Disk
{
public:
    int disk_no;

    uint32_t (*total_sectors)();
    void (*_write_one)(uint32_t sector, uint8_t *data);
    void (*_write)(uint32_t lba, uint8_t sectors, uint8_t *data);
    uint8_t *(*_read)(uint8_t *out, uint32_t sector, uint8_t sectors_to_read);

    int write(uint32_t sector, uint8_t sectors, uint8_t *data);
    int write_one(uint32_t sector, uint8_t *data);
    int read(uint8_t *out, uint32_t sector, uint8_t sectors);
};

Disk *disks[10];
int disk_count = 0;

void load_disk(Disk *disk);
Disk *get_disk(int index);
