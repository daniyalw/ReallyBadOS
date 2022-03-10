#include "dir.h"
#include "node.h"
#include "file.h"

int make_dir(char *name, char *dir, int permission)
{
    fs_node_t *node = create_node(name, dir, FS_FOLDER, permission);

    if (node == NULL)
        return 1;

    return 0;
}

int make_dir(char *name, char *dir)
{
    return make_dir(name, dir, USER_PERMISSION);
}

int make_dir_user(char *name, char *dir)
{
    return make_dir(name, dir, USER_PERMISSION);
}

int kmake_dir(char *name, char *dir)
{
    return make_dir(name, dir, ROOT_PERMISSION);
}

DIR *opendir(char *name)
{
    fs_node_t *node = find_node(name);

    if (node == NULL)
        return NULL;

    DIR *dir = new DIR();
    dir->node = node;

    return dir;
}

fs_node_t *readdir(DIR *dir)
{
    if (dir->ptr == dir->node->children_count)
        return NULL;

    fs_node_t *node = nodes[dir->node->children[dir->ptr]];
    dir->ptr++;

    return node;
}
