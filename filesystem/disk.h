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
    char name[20];
    char path[100];
    int sector;
    int type;
} __attribute__((packed)) RBFSIndex;

Disk *rbfs_disk = NULL;

RBFSIndex **indexed;
int index_count = 0;

void rbfs_format();
void wipe_disk();

RBFSNode *find_file(char *path);
int find_sector_node(RBFSNode *node);
void find_disk();
RBFSIndex *find_index(char *path);
RBFSIndex *find_index(RBFSNode *node);

void move_sector_up(int sector);
void move_sector_down(int sector);
void move_sectors_up(int sector, int sectors);
void move_sectors_down(int sector, int sectors);

void edit_file(RBFSNode *node, char *new_contents);

void add_index(char *name, char *path, int sector, int type);
int rbfs_create(char *name, char *parent, int type, int permission, char *contents);
int rbfs_create_file(char *path, char *contents);
int rbfs_create_dir(char *path);

void index_disk();

char *read_file(RBFSNode *node, int offset, int sectors);
char *read_file(RBFSNode *node);
char *read_file(char *path);

int init_rbfs();
