#include "diskfs.h"

// disk_write_function should have 3 parameters of sector, sector_count, uint8_t*
void format(auto disk_write_function)
{
    disk_fs_master_t *master = (disk_fs_master_t *)malloc(sizeof(disk_fs_master_t *));

    master->magic = FS_MAGIC;
    master->state = FS_NOT_NULL;
    master->free_sector = FS_BEGINNING_SECTOR + 1;

    disk_write_function(FS_BEGINNING_SECTOR, 1, (uint8_t *)master);

    free(master);

    return;
}

// uint8_t *disk_read_function(bytes, lba, sectors);
// disk write same as format
int write_new_file_disk(auto disk_write_function, auto disk_read_function, char *name, char *contents, int type)
{
    uint8_t *bytes = disk_read_function(bytes, FS_BEGINNING_SECTOR, 1);
    disk_fs_master_t *master = (disk_fs_master_t *)bytes;
    int contents_sectors = strlen(contents)/512 + 1;

    printf("Contents sector length: %d\n", contents_sectors);

    disk_fs_node_t *node = (disk_fs_node_t *)malloc(sizeof(disk_fs_node_t));

    strcpy(node->name, name);
    node->type = type;
    node->magic = FS_MAGIC;
    node->state = FS_NOT_NULL;
    node->contents_sectors = contents_sectors;

    disk_write_function(master->free_sector, 1, (uint8_t *)node);
    disk_write_function(master->free_sector + 1, contents_sectors, (uint8_t *)contents);

    master->free_sector++;
    master->free_sector += contents_sectors;

    disk_write_function(FS_BEGINNING_SECTOR, 1, (uint8_t *)master);

    free(node);

    return 0;
}
