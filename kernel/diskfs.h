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
} __attribute__((packed)) disk_master_t;

typedef struct
{
    char name[20];
    char path[100];
    char parent[100];
    int type; // type of node
    int magic; // again, always should be FS_MAGIC
    int state;
    int id;
    int contents_sectors; // how many sectors does the contents take up

    char padding[287];
} __attribute__((packed)) disk_node_t;

disk_t *disk;
int selected_disk = 0;
