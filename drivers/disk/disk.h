#pragma once

typedef struct
{
    int disk_no;

    uint32_t (*total_sectors)();
    void (*write)(uint32_t sector, uint8_t *data);
    uint8_t *(*read)(uint8_t *out, uint32_t sector, uint8_t sectors_to_read);
} disk_t;

disk_t *disks[10];
int disk_count = 0;

void load_disk(disk_t *disk);
disk_t *get_disk(int index);
