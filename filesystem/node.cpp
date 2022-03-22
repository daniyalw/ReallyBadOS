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

fs_node_t *find_node(int fd)
{
    return nodes[fd];
}

// parse file/dir name and get parent id
// good for when the file/dir doesn't exist (or is assumed not to)
int parse_path_file_parent(char *path, char *target)
{
    char **buf;
    int ret = tokenize(path, PATH_SEP, buf);
    int b = 0;

    fs_node_t *node;
    fs_node_t *parent;
    node = root;
    parent = root;

    target = buf[ret-1];

    if ((ret - 1) == 0)
        return 0; // root id

    for (int z = 0; z < ret - 1; z++)
    {
        if (strcmp(buf[z], PATH_UP) == 0)
        {
            if (z == 0)
                continue;

            node = parent;
            parent = nodes[parent->parent_id];
        }
        else if (strcmp(buf[z], PATH_DOT) == 0)
        {
            continue;
        }
        for (int c = 0; c < node->children_count; c++)
        {
            if (strcmp(nodes[node->children[c]]->name, buf[z]) == 0)
            {
                parent = node;
                node = nodes[node->children[c]];
                break;
            }
        }
    }

    return node->id;
}

// parse file/dir name and get current file id
// good for when the file/dir exists (or is assumed to)
int parse_path_file(char *path)
{
    char **buf;
    int ret = tokenize(path, PATH_SEP, buf);
    int b = 0;

    fs_node_t *node;
    fs_node_t *parent;
    node = root;
    parent = root;

    for (int z = 0; z < ret; z++)
    {
        if (strcmp(buf[z], PATH_UP) == 0)
        {
            if (z == 0)
                continue;

            node = parent;
            parent = nodes[parent->parent_id];
        }
        else if (strcmp(buf[z], PATH_DOT) == 0)
        {
            continue;
        }
        for (int c = 0; c < node->children_count; c++)
        {
            if (strcmp(nodes[node->children[c]]->name, buf[z]) == 0)
            {
                parent = node;
                node = nodes[node->children[c]];
                break;
            }
        }
    }

    return node->id;
}

fs_node_t *mount_fs(char *name, char *parent, __write write, __read read, __mkfile mkfile, __mkdir mkdir, int permission)
{
    fs_node_t *node = create_node(name, parent, FS_FOLDER, permission);

    if (node == NULL)
        return NULL;

    node->is_mountpoint = true;
    node->is_mount = true;
    node->mount_parent = node->id;
    node->write = write;
    node->read = read;
    node->mkfile = mkfile;
    node->mkdir = mkdir;
    strcpy(node->mount_dir, name);

    nodes[node->id] = node;

    return node;
}

fs_node_t *create_node(char *name, char *parent_path, int type, int permission, bool ignore_mount)
{
    fs_node_t *parent = find_node(parent_path);

    if (parent == NULL || parent->flags != FS_FOLDER)
        return NULL;

    fs_node_t *node = new fs_node_t();

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

    if (parent->is_mount && ignore_mount == false)
    {
        node->mount_parent = parent->mount_parent;
        node->is_mount = true;
        strcpy(node->mount_dir, parent->mount_dir);

        node->write = parent->write;
        node->read = parent->read;
        node->mkfile = parent->mkfile;

        int ret = -1;

        if (type == FS_FILE)
            ret = node->mkfile(node);
        else if (type == FS_FOLDER)
            ret = node->mkdir(node, node->path);

        if (ret != 0)
        {
            free(node);
            printf("Error creating node");
            return NULL;
        }
    }

    parent->children[parent->children_count] = node->id;
    parent->children_count++;

    nodes[parent->id] = parent;
    nodes[node_count] = node;
    node_count++;

    return node;
}

fs_node_t *create_node_ignore(char *name, char *parent_path, int type, int permission)
{
    return create_node(name, parent_path, type, permission, true);
}

fs_node_t *create_node(char *name, char *parent_path, int type, int permission)
{
    return create_node(name, parent_path, type, permission, false);
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
    {
        free(node->contents);
        node->contents = (char *)malloc(strlen(contents) + 1);
        memset(node->contents, 0, strlen(contents) + 1);
        strcpy(node->contents, contents);
    }
    else
        ret = 1;

    node->size = strlen(contents);
    nodes[node->id] = node;

    return ret;
}

int read_node(fs_node_t *node, int offset, int size, char *buffer)
{
    int ret = 0;

    if (node->read != NULL)
        ret = node->read(node, offset, size, buffer);
    else if (node->flags != FS_FOLDER)
        memcpy(buffer, node->contents, size);

    return ret;
}

int list_dir(fs_node_t *node, int index)
{
    if (node == NULL)
        return 1;

    for (int z = 0; z < index; z++)
        printf("    ");

    for (int z = 0; z < index; z++)
        Kernel::serial_write_string("    ");

    printf("%s\n", node->name);
    Kernel::serial_write_string("%s\n", node->name);

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
