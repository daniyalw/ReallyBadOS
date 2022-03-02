#pragma once

#define FS_MAGIC 0x12345

#define FS_STATE_NORMAL 0x1
#define FS_STATE_ERROR 0x2

#define FS_NULL 2
#define FS_NOT_NULL 1

typedef struct
{
    // should always be FS_MAGIC
    int magic;
    int state;
    int null; // should always be 0
    int first_available_sector;
} __attribute__((packed)) fs_first_sector_t;

typedef struct
{
    char name[20];
    char path[100];

    int sector_beginning; // offset from beginning of eleventh sector (the sector where this tracker does not track)

    char padding[7]; // to make 128 bytes
    int null;
} __attribute__((packed)) fs_node_tracker_t;

typedef struct
{
    char name[20];
    char path[100];
    int contents_sectors;

    char padding[390]; // leave one byte off
} __attribute__((packed)) fs_disk_file_t;

typedef struct
{
    fs_node_tracker_t nodes[4 * 10]; // 4 node trackers per sector, 10 sectors
    int node_count = 0;
    char padding[39];
} __attribute__((packed)) fs_global_tracker_t;

void format();
int create_file(char *name, char *path, char *contents);
