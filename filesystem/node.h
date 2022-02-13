#pragma once

#define FS_NODE_FOLDER 2
#define FS_NODE_FILE 3

typedef void (*__write)(char *);
typedef char * (*__read)(char *);

struct fs_node
{
    char name[20];
    char path[100];
    int id;
    int parent_id;
    int flags;
    bool null = false;
    int size;

    int children_id[10]; // if the node is a folder
    int children_count = 0;

    char *contents; // if it is a file

    __write write = NULL;
    __read read = NULL;
};

fs_node nodes[1000];
int node_count = 0;

fs_node root;

fs_node null_node();
fs_node find_node(int id);
fs_node create_node(char *name, int parent, int flags);
void fs_ls(int id, int index);
void node_write_basic(int id, char *contents);
char *node_read_basic(int id);
char *find_name(int id);
int find_id(char *path);
void ls_root();
void fs_ls(char *path);
