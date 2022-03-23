#pragma once

class Disk
{
public:
    int disk_no;

    uint32_t (*total_sectors)();
    void (*write)(uint32_t sector, uint8_t *data);
    uint8_t *(*read)(uint8_t *out, uint32_t sector, uint8_t sectors_to_read);
};

Disk *disks[10];
int disk_count = 0;

void load_disk(Disk *disk);
Disk *get_disk(int index);
