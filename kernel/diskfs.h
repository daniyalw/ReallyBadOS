#pragma once

#include <disk/ata.h>

#define FS_MAGIC 0x12345

#define FS_STATE_NORMAL 0x1
#define FS_STATE_ERROR 0x2

#define FS_NULL 2
#define FS_NOT_NULL 1

#define FS_BEGINNING_SECTOR 3 // leave 2 sectors for boot

typedef struct
{
    int magic; // should always be FS_MAGIC
    int state;
    int free_sector; // first free sector
    int files;

    char padding[508];
} disk_fs_master_t;

typedef struct
{
    char name[20];
    char path[100];
    int type; // type of node
    int magic; // again, always should be FS_MAGIC
    int state;
    int contents_sectors; // how many sectors does the contents take up

    int offset = 0;

    char padding[387];
} disk_fs_node_t;

typedef uint8_t* (*disk_read_t)(uint8_t*, uint32_t, uint8_t);
typedef void (*disk_write_t)(uint32_t, uint8_t, uint8_t*);

disk_read_t disk_read = ata_read;
disk_write_t disk_write = ata_write;

void format();
disk_fs_master_t *get_master();

int write_new_file_disk(char *name, char *path, char *contents, int type);

void edit_disk_file(disk_fs_node_t *node, char *new_contents);
void edit_disk_file(char *path, char *new_contents);

void move_sector_up(int beginning_sector);
void move_sectors_up(int beginning_sector, int sectors);
void move_sector_down(int sector);
void move_sectors_down(int sector, int sectors);

int find_offset(disk_fs_node_t *node);
int find_sector(disk_fs_node_t *node);

char *read_file_sectors(disk_fs_node_t *node, int sector, int sectors_to_read, char *buf, int bufsize);
disk_fs_node_t *disk_file_open(char *path);
