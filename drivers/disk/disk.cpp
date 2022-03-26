#include <drivers/disk/disk.h>

void load_disk(Disk *disk) {
    disk->disk_no = disk_count;

    disks[disk_count] = disk;
    disk_count++;
}

Disk *get_disk(int index) {
    if (index < 0 || index > disk_count)
        return NULL;

    return disks[index];
}
