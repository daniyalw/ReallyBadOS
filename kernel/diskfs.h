#pragma once

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

    char padding[509];
} disk_fs_master_t;

typedef struct
{
    char name[20];
    int type; // type of node
    int magic; // again, always should be FS_MAGIC
    int state;
    int contents_sectors; // how many sectors does the contents take up

    char padding[488];
} disk_fs_node_t;

void format();
int write_new_file_disk(auto disk_write_function, auto disk_read_function, char *name, char *contents, int type);
