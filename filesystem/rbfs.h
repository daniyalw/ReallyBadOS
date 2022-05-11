#pragma once

#define RBFS_BEG 3
#define RBFS_DISK_MAGIC 0x52424653 // 'RBFS'

#define RBFS_CLEAN 0
#define RBFS_ERROR 1

#define RBFS_DIR 0
#define RBFS_FILE 1

#define RBFS_MOUNT "/disk0"

#define RBFS_PERM_ROOT 1
#define RBFS_PERM_USER 0

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
    char path[PATH_LIMIT]; // entire path
    int uid; // user id
    int gid; // group id
    int error; // clean - 0; error - 1
    int permission; // what permissions does file have
    int sectors; // how many sectors used by the contents of node; if dir, it means 0
    int type; // dir - 0; file - 1
    time_t ctime;
} __attribute__((packed)) RBFSNode;

typedef struct {
    uint32_t magic;
    char name[20];
    char path[PATH_LIMIT];
    int sector;
    int sectors;
    int type;
    int id;
    int permission;
    time_t ctime;
} __attribute__((packed)) RBFSIndex;

RBFSIndex *indexed[1000];
int index_count = 0;

namespace rbfs {

void format();
void init();

RBFSIndex *find_index(char *path);
void print_indexes();
void add_index(RBFSNode *node, int offset);
void add_index(RBFSNode node, int offset);
void index_disk();
void rescan();

void str_from_ustr(char *str, uint8_t *ustr, int size);
void ustr_from_str(uint8_t *out, char *str, int size);

int add_node(char *path, int type, int perm, char *contents);
int create_file(char *path, char *contents);
int create_file_auth(char *path, char *contents);
int create_folder(char *path);

void move_sector_up(int sector);
void move_sector_down(int sector);
void sectors_up(int sector, int sectors);
void sectors_down(int sector, int sectors);

void modify_file(RBFSIndex *index, char *contents);
RBFSIndex *open(char *path);
RBFSIndex *open_root(char *path);
void delete_node(RBFSIndex *index);
int read(char *out, int offset, int size, RBFSIndex *index);
int write(char *buf, int offset, int size, RBFSIndex *index);

int vfs_write(fs_node_t *node, int offset, int size, char *buf);
int vfs_read(fs_node_t *node, int offset, int size, char *buf);
int vfs_mkfile(fs_node_t *node);
int vfs_mkdir(fs_node_t *node);

}
