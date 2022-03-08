#include <filesystem/node.h>

fs_node_t *find_node(char *path)
{
    for (int z = 0; z < node_count; z++)
    {
        fs_node_t *node = nodes[z];

        if (strcmp(node->path, path) == 0)
            return node;
    }

    return NULL;
}

fs_node_t *create_node(char *name, char *parent_path, int type, int permission)
{
    fs_node_t *parent = find_node(parent_path);

    if (parent == NULL || parent->flags != FS_FOLDER)
        return NULL;

    fs_node_t *node = new fs_node_t(); // yes, i know

    memset(node->name, 0, FILENAME_LIMIT);
    memset(node->path, 0, FILENAME_LIMIT * 10);

    strcpy(node->name, name);
    if (type == FS_FILE)
        strcpy(node->path, get("", "%s%s", parent_path, name));
    else
        strcpy(node->path, get("", "%s%s/", parent_path, name));
    node->id = node_count;
    node->parent_id = parent->id;
    node->flags = type;

    node->permission = permission;

    parent->children[parent->children_count] = node->id;
    parent->children_count++;

    nodes[parent->id] = parent;
    nodes[node_count] = node;
    node_count++;

    return node;
}

void close_node(fs_node_t *node)
{
    free(node);
}

int write_node(fs_node_t *node, int offset, int size, char *contents)
{
    int ret = 0;

    if (node->write != NULL)
        ret = node->write(node, offset, size, contents);
    else if (node->flags != FS_FOLDER)
        node->contents = contents;
    else
        ret = 1;

    nodes[node->id] = node;

    return ret;
}

char *read_node(fs_node_t *node, int offset, int size, char *buffer)
{
    if (node->read != NULL)
        buffer = node->read(node, offset, size, buffer);
    else if (node->flags != FS_FOLDER)
        buffer = node->contents;

    for (int z = 0; z < size; z++)
    {
        buffer[z] = buffer[z + offset];
    }

    return buffer;
}

int list_dir(fs_node_t *node, int index)
{
    if (node == NULL)
        return 1;

    for (int z = 0; z < index; z++)
        printf("    ");

    printf("%s\n", node->name);

    for (int z = 0; z < node->children_count; z++)
        list_dir(nodes[node->children[z]], index + 1);

    return 0;
}

int list_dir(char *dir)
{
    fs_node_t *node = find_node(dir);

    if (node == NULL || node->flags == FS_FILE)
        return 1;

    int ret = list_dir(node, 0);

    return ret;
}

void create_root()
{
    root = new fs_node_t();

    memset(root->name, 0, FILENAME_LIMIT);
    memset(root->path, 0, FILENAME_LIMIT * 10);

    strcpy(root->name, "/");
    strcpy(root->path, "/");

    root->id = 0;
    root->parent_id = -1;

    root->flags = FS_FOLDER;

    nodes[0] = root;

    node_count++;
}

void init_fs()
{
    create_root();

    make_dir("bin", "/");
    make_dir("usr", "/");
    make_dir("dev", "/");
    make_dir("apps", "/");

    make_dir("bin", "/usr/");
    make_dir("documents", "/usr/");
    make_dir("scripts", "/usr/");
    make_dir("fonts", "/usr/");

    log::info("Initialized virtual file system");
}
