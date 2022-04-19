#pragma once

#define RBFS_BEG 3
#define RBFS_DISK_MAGIC 0x52424653 // 'RBFS'

#define RBFS_CLEAN 0
#define RBFS_ERROR 1

#define RBFS_DIR 0
#define RBFS_FILE 1

typedef struct {
    uint32_t magic; // should always be DISK_MAGIC
    uint32_t disk_size; // the size of the disk
    int status; // clean - 0; error - 1
    int files;
    int first_free;
} __attribute__((packed)) RBFSSuperblock;

typedef struct {
    uint32_t magic;
    char name[20]; // name without the rest of the path
    char path[100]; // entire path
    int uid; // user id
    int gid; // group id
    int error; // clean - 0; error - 1
    int permission; // what permissions does file have
    int sectors; // how many sectors used by the contents of node; if dir, it means 0
    int type; // dir - 0; file - 1
} __attribute__((packed)) RBFSNode;

typedef struct {
    uint32_t magic;
    char name[20];
    char path[100];
    int sector;
    int sectors;
    int type;
    bool null = false;
} __attribute__((packed)) RBFSIndex;

Disk *rbfs_disk = NULL;

RBFSIndex *indexed[1000];
int index_count = 0;
