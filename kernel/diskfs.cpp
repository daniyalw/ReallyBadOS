#include "diskfs.h"

int format()
{
    uint8_t *bytes;
    bytes = disk->read(bytes, FS_BEGINNING_SECTOR, 1);

    disk_master_t *tmp_master = (disk_master_t *)bytes;

    if (tmp_master->magic == FS_MAGIC)
    {
        log::warning("diskfs: disk %d already formatted with filesystem", selected_disk);
        return 1;
    }

    disk_master_t *master = (disk_master_t *)malloc(sizeof(disk_master_t));

    if (master == NULL)
    {
        log::warning("diskfs: Not enough space for disk_master_t*");
        return 1;
    }

    master->magic = FS_MAGIC;
    master->state = FS_NOT_NULL;
    master->free_sector = FS_BEGINNING_SECTOR + 1;
    master->files = 0;

    disk->write(FS_BEGINNING_SECTOR, (uint8_t *)master);

    free(master); // already wrote to disk so we don't need it

    return 0;
}

int disk_write(char *fname, char *fparent, char *fpath, int type, int id, char *contents)
{
    uint8_t *bytes;
    bytes = disk->read(bytes, FS_BEGINNING_SECTOR, 1);

    disk_master_t *master = (disk_master_t *)bytes;

    if (master->magic != FS_MAGIC)
    {
        log::warning("diskfs: invalid magic");
        return 1;
    }

    disk_node_t *node = (disk_node_t *)malloc(sizeof(disk_node_t));

    if (node == NULL)
    {
        log::warning("diskfs: not enough space for disk_node_t*");
        return 1;
    }

    int sectors = strlen(contents)/512 + 1;

    memset(node->name, 0, 20);
    memset(node->path, 0, 100);
    memset(node->parent, 0, 100);

    strcpy(node->name, fname);
    strcpy(node->path, fpath);
    strcpy(node->parent, fparent);

    node->type = type;
    node->magic = FS_MAGIC;
    node->state = FS_NOT_NULL;
    node->contents_sectors = sectors;
    node->id = id;

    disk->write(master->free_sector, (uint8_t *)node);

    master->free_sector + 1;
    master->free_sector += sectors;

    disk->write(FS_BEGINNING_SECTOR, (uint8_t *)master);

    disk->write(master->free_sector - sectors, (uint8_t *)contents);

    return 0;
}

disk_node_t *disk_find_node(uint8_t *bytes, char *path)
{
    bytes = disk->read(bytes, FS_BEGINNING_SECTOR, 1);

    disk_master_t *master = (disk_master_t *)bytes;

    if (master->magic != FS_MAGIC)
    {
        log::warning("diskfs: invalid master magic");
        return NULL;
    }

    int top = master->free_sector;
    int reach = 0;

    for (int sector = FS_BEGINNING_SECTOR; sector < top; sector++)
    {
        if (reach == 0)
        {
            bytes = disk->read(bytes, sector, 1);
            disk_node_t *node = (disk_node_t *)bytes;

            if (node->magic != FS_MAGIC)
            {
                log::warning("diskfs: invalid node magic (a)");
                return NULL;
            }

            if (strcmp(node->path, path) == 0)
                return node;

            reach = node->contents_sectors;
        }
        else
        {
            reach--;
        }
    }

    return NULL;
}

disk_node_t *disk_open(char *path)
{
    uint8_t *bytes;
    return disk_find_node(bytes, path);
}

int disk_sector_down(int sector)
{
    uint8_t *bytes;
    bytes = disk->read(bytes, FS_BEGINNING_SECTOR, 1);

    disk_master_t *master = (disk_master_t *)bytes;

    if (master->magic != FS_MAGIC)
    {
        log::warning("diskfs: invalid master magic");
        return NULL;
    }

    int top = master->free_sector;

    for (int z = top; z > sector; z--)
    {
        bytes = disk->read(bytes, z, 1);
        disk->write(z - 1, bytes);
    }

    master->free_sector--;

    disk->write(FS_BEGINNING_SECTOR, (uint8_t *)master);

    return 0;
}

int disk_sector_up(int sector)
{
    uint8_t *bytes;
    bytes = disk->read(bytes, FS_BEGINNING_SECTOR, 1);

    disk_master_t *master = (disk_master_t *)bytes;

    if (master->magic != FS_MAGIC)
    {
        log::warning("diskfs: invalid master magic");
        return NULL;
    }

    int top = master->free_sector;

    for (int z = top; z >= sector; z--)
    {
        bytes = disk->read(bytes, z, 1);
        disk->write(z + 1, bytes);
    }

    return 0;
}

void disk_up_sectors(int sector, int sectors)
{
    for (int z = 0; z < sectors; z++)
        disk_sector_up(sector + z);
}

void disk_down_sectors(int sector, int sectors)
{
    for (int z = sectors; z >= 0; z--)
        disk_sector_down(sector + z);
}

// offset is beginning sector of contents
void edit_file_size(disk_node_t *node, int offset, char *nc)
{
    int sectors = strlen(nc)/512 + 1;

    if (sectors < node->contents_sectors)
        disk_down_sectors(offset + (node->contents_sectors - sectors), node->contents_sectors - sectors);
    else if (sectors > node->contents_sectors)
        disk_up_sectors(offset + node->contents_sectors, sectors - node->contents_sectors);

    disk->write(offset, (uint8_t *)nc);
}

int disk_find_sector(disk_node_t *node)
{
    uint8_t *bytes;
    bytes = disk->read(bytes, FS_BEGINNING_SECTOR, 1);

    disk_master_t *master = (disk_master_t *)bytes;

    if (master->magic != FS_MAGIC)
    {
        log::warning("diskfs: invalid master magic");
        return NULL;
    }

    int top = master->free_sector;
    int reached = 0;

    for (int z = FS_BEGINNING_SECTOR + 1; z < top; z++)
    {
        if (reached == 0)
        {
            bytes = disk->read(bytes, z, 1);
            disk_node_t *_node = (disk_node_t *)bytes;

            if (_node->magic != FS_MAGIC)
            {
                log::warning("diskfs: invalid node magic");
                return -1;
            }

            if (strcmp(_node->path, node->path) == 0)
                return z;

            reached = _node->contents_sectors;
        }
        else
        {
            reached--;
        }
    }

    return -1;
}

int disk_find_sector(char *path)
{
    disk_node_t *node = disk_open(path);

    if (node == NULL)
        return -1;

    return disk_find_sector(node);
}

char *disk_read(disk_node_t *node, int size, char *buf)
{
    int sector = disk_find_sector(node);

    if (sector == -1)
        return NULL;

    uint8_t *bytes;
    bytes = disk->read(bytes, sector + 1, size/512 + 1);

    memset(buf, 0, size + 1);

    for (int z = 0; z < size; z++)
        buf[z] = (char)bytes[z];

    return buf;
}

int write_vfs_disk(fs_node_t *node, int offset, int size, char *buf)
{
    return 0;
}

int read_vfs_disk(fs_node_t *node, int offset, int size, char *buf)
{
    return 0;
}

int true_mkfile_vfs_disk(fs_node_t *node, __read read, __write write)
{
    return 0;
}

int tmp_mkfile_vfs_disk(fs_node_t *node, __read read, __write write)
{
    return 0;
}

void disk_init(fs_node_t *disk0)
{
    disk = get_disk(selected_disk);


    if (disk == NULL)
        log::error("diskfs: Disk %d not found.", selected_disk);
    else
        log::info("diskfs: disk %d successfully initialized!", selected_disk);
}
