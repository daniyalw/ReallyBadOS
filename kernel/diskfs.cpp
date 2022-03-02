#include "diskfs.h"

// disk_write_function should have 3 parameters of sector, sector_count, uint8_t*
void format(auto disk_write_function)
{
    fs_first_sector_t *s = (fs_first_sector_t *)malloc(sizeof(fs_first_sector_t *));
    s->magic = FS_MAGIC;
    s->null = 0;
    s->state = FS_STATE_NORMAL;
    s->first_available_sector = 10;

    disk_write_function(0, 1, (uint8_t *)s);
    free(s);

    uint32_t addr = malloc(sizeof(fs_global_tracker_t *));
    fs_global_tracker_t *tracker = (fs_global_tracker_t *)addr;
    memset((int *)addr, 0, 512 * 10);
    tracker->node_count = 0;

    for (int z = 0; z < 40; z++)
    {
        auto node = tracker->nodes[z];
        node.null = FS_NULL;
        tracker->nodes[z] = node;
    }

    disk_write_function(1, 10, (uint8_t *)tracker);

    free(tracker);
}

// uint8_t* disk_read(uint8_t *buf, int sector, int sectors_to_read)
// void disk_write(int sector, int sectors_to_write, uint8_t* buf)
int create_file(auto disk_read, auto disk_write, char *name, char *path, char *contents)
{
    uint8_t *bytes;
    bytes = disk_read(bytes, 0, 1);

    fs_first_sector_t *fsector = (fs_first_sector_t *)bytes;

    if (fsector->magic != FS_MAGIC)
    {
        log::error("Cannot create disk file: first sector magic not valid!");
        return 1;
    }

    const int contents_sectors = strlen(contents)/512 + 1;

    // actually create the file and write it to disk
    fs_disk_file_t *file = (fs_disk_file_t *)malloc(512); // just the right size

    strcpy(file->name, name);
    strcpy(file->path, path);
    file->contents_sectors = contents_sectors;

    disk_write(fsector->first_available_sector, 1, (uint8_t *)file);
    free(file);

    disk_write(fsector->first_available_sector + 1, contents_sectors, (uint8_t *)contents); // now write the contents to disk

    fsector->first_available_sector += 1 + contents_sectors;

    disk_write(0, 1, (uint8_t *)fsector); // flush new data (the first_available_sector) to disk

    // now create a tracker
    bytes = disk_read(bytes, 1, 10);
    fs_global_tracker_t *tracker = (fs_global_tracker_t *)bytes;

    fs_node_tracker_t node = tracker->nodes[tracker->node_count];
    strcpy(node.name, name);
    strcpy(node.path, path);
    node.sector_beginning = fsector->first_available_sector - 1 - contents_sectors;

    tracker->nodes[tracker->node_count] = node;
    tracker->node_count++;

    // write tracker to disk
    disk_write(1, 10, (uint8_t *)tracker);

    return 0;
}
