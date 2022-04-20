#pragma once

#define FILENAME_LIMIT 20
#define CHILDREN_LIMIT 20

#define ROOT_PERMISSION 0
#define USER_PERMISSION 1

#define PATH_UP ".."
#define PATH_DOT "."
#define PATH_SEP "/"

#define NODE_LIMIT 1000

#define FS_FOLDER 0x1
#define FS_FILE 0x2

struct fs_node_t;

typedef int (*__write)(fs_node_t*, int, int, char *);
typedef int (*__read)(fs_node_t*, int, int, char *);
typedef int (*__mkfile)(fs_node_t*);
typedef int (*__mkdir)(fs_node_t *);

struct fs_node_t
{
    char name[FILENAME_LIMIT];
    char path[FILENAME_LIMIT * 10];

    int id, parent_id;
    int flags;

    int permission;

    bool null = false;

    int size = 0;
    char *contents;

    bool is_mountpoint;
    char mount_dir[FILENAME_LIMIT];
    int mount_parent;
    bool is_mount;
    __mkfile mkfile = NULL;

    __write write = NULL;
    __read read = NULL;
    __mkdir mkdir = NULL;

    int children[CHILDREN_LIMIT];
    int children_count = NULL;
};

fs_node_t *nodes[NODE_LIMIT];
int node_count = 0;

fs_node_t *root;

fs_node_t *mount_fs(char *name, char *parent, __write write, __read read, __mkfile mkfile, __mkdir mkdir, int permission);

fs_node_t *find_node(char *path);
fs_node_t *find_node(int fd);

void close_node(fs_node_t *node);
int write_node(fs_node_t *node, int offset, int size, char *contents);
int read_node(fs_node_t *node, int offset, int size, char *buffer);

fs_node_t *create_node_ignore(char *name, char *parent_path, int type, int permission);
fs_node_t *create_node(char *name, char *parent_path, int type, int permission, bool ignore_mount);
fs_node_t *create_node(char *name, char *parent_path, int type, int permission);

int list_dir(fs_node_t *node, int index, bool serial_only);
int list_dir(char *dir);
int list_dir_serial(char *dir);

void create_root();
void init_fs();

int parse_path_file(char *path);
int parse_path_file_parent(char *path);

fs_node_t *find_node_fixed(char *path);
char *find_fixed(char *path);
fs_node_t *absolute_path_node(char *cwd, char *fmt);
char *absolute_path(char *cwd, char *fmt);

char *get_type(fs_node_t *node);

void delete_node(fs_node_t *node);
void delete_node(char *path);
fs_node_t *_clone_node(fs_node_t *node);
fs_node_t *copy_node(fs_node_t *node, char *new_path);
fs_node_t *copy_node(char *old_path, char *new_path);
fs_node_t *move_node(fs_node_t *node, char *new_path);
fs_node_t *move_node(char *old, char *new_path);
