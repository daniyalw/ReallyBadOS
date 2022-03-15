#include "diskfs.h"

// disk_write_function should have 3 parameters of sector, sector_count, uint8_t*
void format()
{
    disk_fs_master_t *master = (disk_fs_master_t *)malloc(sizeof(disk_fs_master_t *));

    master->magic = FS_MAGIC;
    master->state = FS_NOT_NULL;
    master->free_sector = 4;
    master->files = 0;

    disk_write(FS_BEGINNING_SECTOR, 1, (uint8_t *)master);

    free(master);

    return;
}

disk_fs_master_t *get_master()
{
    uint8_t *bytes;
    bytes = disk_read(bytes, FS_BEGINNING_SECTOR, 1);
    disk_fs_master_t *master = (disk_fs_master_t *)bytes;
    return master;
}

// uint8_t *disk_read_function(bytes, lba, sectors);
// disk write same as format
int write_new_file_disk(char *name, char *path, char *contents, int type)
{
    disk_fs_master_t *master = get_master();
    int contents_sectors = strlen(contents)/512 + 1;

    disk_fs_node_t *node = (disk_fs_node_t *)malloc(sizeof(disk_fs_node_t));

    strcpy(node->name, name);
    strcpy(node->path, path);
    node->type = type;
    node->magic = FS_MAGIC;
    node->state = FS_NOT_NULL;
    node->contents_sectors = contents_sectors;

    node->offset = master->free_sector;

    disk_write(master->free_sector, 1, (uint8_t *)node);
    disk_write(master->free_sector + 1, contents_sectors, (uint8_t *)contents);

    master->free_sector++;
    master->free_sector += contents_sectors;
    master->files++;

    disk_write(FS_BEGINNING_SECTOR, 1, (uint8_t *)master);

    free(node);

    return 0;
}

void edit_disk_file(disk_fs_node_t *node, char *new_contents)
{
    int contents_sectors = strlen(new_contents)/512 + 1;
    int old_sectors = node->contents_sectors;

    if (contents_sectors > old_sectors)
    {
        move_sectors_up(node->offset + old_sectors + 1, contents_sectors - old_sectors);
        node->contents_sectors = contents_sectors;

        disk_write(node->offset, 1, (uint8_t *)node);
        disk_write(node->offset + 1, contents_sectors, (uint8_t *)new_contents);
    }
    else if (contents_sectors < old_sectors)
    {
        move_sectors_down(node->offset + 1 + old_sectors, old_sectors - contents_sectors);
        node->contents_sectors = contents_sectors;

        disk_write(node->offset, 1, (uint8_t *)node);
        disk_write(node->offset + 1, contents_sectors, (uint8_t *)new_contents);
    }
    else
    {
        printf("a");
        putchar('\b', 0x0F00);
        disk_write(node->offset + 1, old_sectors, (uint8_t *)new_contents);
    }
}

void edit_disk_file(char *path, char *new_contents)
{
    disk_fs_node_t *node = disk_file_open(path);
    edit_disk_file(node, new_contents);
}

void move_sector_up(int beginning_sector)
{
    disk_fs_master_t *master = get_master();

    int top = master->free_sector - 1;

    for (int z = top; z >= beginning_sector; z--)
    {
        uint8_t *bytes;
        bytes = disk_read(bytes, z, 1);

        disk_fs_node_t *node = (disk_fs_node_t *)bytes;

        if (node->magic == FS_MAGIC)
        {
            node->offset++;
            disk_write(z + 1, 1, (uint8_t *)node);
        }
        else
        {
            disk_write(z + 1, 1, bytes);
        }
    }
}

void move_sectors_up(int beginning_sector, int sectors)
{
    for (int z = 0; z < sectors; z++)
    {
        move_sector_up(beginning_sector);
        beginning_sector++;
    }
}

// sector parameter is the sector that is no longer needed
void move_sector_down(int sector)
{
    disk_fs_master_t *master = get_master();

    int top = master->free_sector;

    for (int z = sector; z < top; z++)
    {
        uint8_t *bytes;
        bytes = disk_read(bytes, z + 1, 1);

        disk_fs_node_t *node = (disk_fs_node_t *)bytes;

        if (node->magic == FS_MAGIC)
        {
            node->offset--;
            disk_write(z, 1, (uint8_t *)node);
        }
        else
        {
            disk_write(z, 1, bytes);
        }
    }
}

void move_sectors_down(int sector, int sectors)
{
    for (int z = sector + sectors; z >= sector; z--)
        move_sector_down(z);
}

int find_offset(disk_fs_node_t *node)
{
    return node->offset;
}

int find_sector(disk_fs_node_t *node)
{
    return node->offset;
}

// sectors start at 0
char *read_file_sectors(disk_fs_node_t *node, int sector, int sectors_to_read, char *buf, int bufsize)
{
    uint8_t *bytes = (uint8_t *)malloc(512 * sizeof(uint8_t));
    disk_read(bytes, node->offset, 1);

    disk_fs_node_t *_node = (disk_fs_node_t *)bytes;

    bytes = disk_read(bytes, node->offset + sector + 1, sectors_to_read);

    for (int z = 0; z < bufsize; z++)
        buf[z] = (char)bytes[z];

    free(bytes);

    return buf;
}

char *read_file_sectors(disk_fs_node_t *node, char *buf)
{
    return read_file_sectors(node, 0, node->contents_sectors, buf, node->contents_sectors * 512);
}

disk_fs_node_t *disk_file_open(char *path)
{
    disk_fs_master_t *master = get_master();

    if (master->magic != FS_MAGIC)
    {
        printf("Master sector not FS_MAGIC!\n");
        return NULL;
    }

    int top = master->free_sector;

    for (int z = FS_BEGINNING_SECTOR + 1; z < top; z++)
    {
        uint8_t *bytes;
        bytes = disk_read(bytes, z, 1);

        disk_fs_node_t *node = (disk_fs_node_t *)bytes;

        if (node->magic == FS_MAGIC && strcmp(node->path, path) == 0)
            return node;
    }


    return NULL;
}

int write_vfs_disk(fs_node_t *node, int offset, int size, char *buf)
{
    char target[100];
    sscanf(node->path, "/disk0%s", &target);
    disk_fs_node_t *_node = disk_file_open(target);

    if (_node == NULL)
        return 1;

    edit_disk_file(_node, buf);

    return 0;
}

char *read_vfs_disk(fs_node_t *node, int offset, int size, char *buf)
{
    disk_fs_node_t *_node = disk_file_open("/hello.txt");

    if (_node == NULL)
    {
        printf("_node is null.\n");
        return NULL;
    }

    read_file_sectors(_node, 0, 1, buf, size);
    return buf;
}

int true_mkfile_vfs_disk(fs_node_t *node, __read read, __write write)
{
    return write_new_file_disk(node->name, node->path, "", 1);
}

int tmp_mkfile_vfs_disk(fs_node_t *node, __read read, __write write)
{
    return 0;
}

void disk_init(fs_node_t *disk0)
{
    printf("b");
    disk_write(0, 1, (uint8_t *)"fuck");
    // if we don't do this, the fucking thing will just division-by-zero fault
    printf("a");

    if (disk0 == NULL)
    {
        printf("Failed to mount disk.\n");
        return;
    }

    disk_fs_master_t *master = get_master();

    if (master->magic != FS_MAGIC)
    {
        printf("Master magic != FS magic!\n");
        return;
    }

    int sector = FS_BEGINNING_SECTOR + 1;

    while (true)
    {
        if (sector >= master->free_sector)
            break;

        uint8_t *bytes;
        bytes = disk_read(bytes, sector, 1);
        disk_fs_node_t *node = (disk_fs_node_t *)bytes;
        printf("File found: %s\n", node->path);

        sector += 1 + node->contents_sectors;

        create_file(node->name, "/disk0/");
    }

    disk0->mkfile = true_mkfile_vfs_disk;
    nodes[disk0->id] = disk0;
}
