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

int Disk::write(uint32_t sector, uint8_t sectors, uint8_t *data) {
    if ((sector + sectors) >= total_sectors()) {
        return 1;
    }

    _write(sector, sectors, data);
    return 0;
}

int Disk::write_one(uint32_t sector, uint8_t *data) {
    if ((sector + sectors) >= total_sectors()) {
        return 1;
    }

    _write_one(sector, data);
    return 0;
}

int Disk::read(uint8_t *out, uint32_t sector, uint8_t sectors) {
    if ((sector + sectors) >= total_sectors()) {
        return 1;
    }

    _read(out, sector, sectors);
    return 0;
}
