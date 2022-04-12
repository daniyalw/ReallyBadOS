#include <filesystem/node.h>
#include <errno.h>
#include "utils.h"

fs_node_t *find_node(char *path) {
    for (int z = 0; z < node_count; z++) {
        fs_node_t *node = nodes[z];

        if (strcmp(node->path, path) == 0) {
            return node;
        }
    }

    return NULL;
}

fs_node_t *find_node(int fd) {
    return nodes[fd];
}

fs_node_t *find_node_fixed(char *path) {
    char **buf;
    fs_node_t *node = nodes[0]; // root dir
    int ret = tokenize(path, '/', buf);

    for (int z = 1; z < ret; z++) {
        bool found = false;

        if (strcmp(buf[z], PATH_UP) == 0) {
            node = nodes[node->parent_id];
            free(buf[z]);
            continue;
        } else if (strcmp(buf[z], PATH_DOT) == 0) {
            free(buf[z]);
            continue;
        } else if (strcmp(buf[z], PATH_SEP) == 0) {
            free(buf[z]);
            continue;
        }

        for (int c = 0; c < node->children_count; c++) {
            fs_node_t *_n = nodes[node->children[c]];

            if (strcmp(_n->name, buf[z]) == 0) {
                node = _n;
                found = true;
                break;
            }
        }

        if (!found) {
            free(buf[z]);
            errno = ENOENT;
            return NULL;
        }

        free(buf[z]);
    }

    return node;
}

char *find_fixed(char *path) {
    fs_node_t *node = find_node_fixed(path);
    return node->path;
}

fs_node_t *absolute_path_node(char *cwd, char *fmt) {
    char path[100];
    memset(path, 0, 100);

    if (cwd[strlen(cwd) - 1] != '/') {
        get(cwd, "%s/", cwd);
    }

    if (fmt[0] != '/') {
        append(cwd, fmt, path);
    } else {
        strcpy(path, fmt);
    }

    fs_node_t *node = find_node_fixed(path);

    return node;
}

char *absolute_path(char *cwd, char *fmt) {
    return absolute_path_node(cwd, fmt)->path;
}

fs_node_t *mount_fs(char *name, char *parent, __write write, __read read, __mkfile mkfile, __mkdir mkdir, int permission) {
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

void delete_node(fs_node_t *node) {
    if (node && node->parent_id >= 0 && node->parent_id < node->id) {
        fs_node_t *parent = nodes[node->parent_id];

        // remove the id from its parent
        int pos = -1;

        for (int z = 0; z < parent->children_count; z++) {
            if (parent->children[z] == node->id) {
                pos = z;
                break;
            }
        }

        // ensure that the parent actually has the child
        if (pos >= 0) {
            for (int z = pos; z < parent->children_count; z++) {
                parent->children[z] = parent->children[z];
            }

            parent->children_count--;
        }

        memset(node, 0, sizeof(fs_node_t));

        delete node;
    }
}

void delete_node(char *path) {
    delete_node(find_node(path));
}

fs_node_t *_clone_node(fs_node_t *node) {
    fs_node_t *copy = new fs_node_t();

    memset(copy, 0, sizeof(fs_node_t));
    memcpy(copy, node, sizeof(fs_node_t));

    return copy;
}

fs_node_t *copy_node(fs_node_t *node, char *new_path) {
    char *parent = find_parent(new_path);
    char *name = find_name(new_path);

    fs_node_t *_node = create_node(name, parent, node->flags, node->permission, false);
    _node->contents = node->contents;
    _node->size = node->size;

    free(parent);
    free(name);

    return _node;
}

fs_node_t *move_node(fs_node_t *node, char *new_path) {
    auto _node = copy_node(node, new_path);
    delete_node(node);
    return _node;
}

fs_node_t *move_node(char *old, char *new_path) {
    return move_node(find_node(old), new_path);
}

fs_node_t *copy_node(char *old_path, char *new_path) {
    return copy_node(find_node(old_path), new_path);
}

fs_node_t *create_node(char *name, char *parent_path, int type, int permission, bool ignore_mount) {
    fs_node_t *parent = find_node(parent_path);

    if (parent == NULL || parent->flags != FS_FOLDER)
        return NULL;

    fs_node_t *node = new fs_node_t();

    memset(node->name, 0, FILENAME_LIMIT);
    memset(node->path, 0, FILENAME_LIMIT * 10);

    strcpy(node->name, name);

    if (strcmp(parent_path, "/") == 0) {
        strcpy(node->path, get("", "%s%s", parent_path, name));
    } else {
        strcpy(node->path, get("", "%s/%s", parent_path, name));
    }

    node->id = node_count;
    node->parent_id = parent->id;
    node->flags = type;
    node->permission = permission;

    if (parent->is_mount && ignore_mount == false) {
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
            ret = node->mkdir(node);

        if (ret != 0) {
            free(node);
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

fs_node_t *create_node_ignore(char *name, char *parent_path, int type, int permission) {
    return create_node(name, parent_path, type, permission, true);
}

fs_node_t *create_node(char *name, char *parent_path, int type, int permission) {
    return create_node(name, parent_path, type, permission, false);
}

void close_node(fs_node_t *node) {
    free(node);
}

int write_node(fs_node_t *node, int offset, int size, char *contents) {
    int ret = 0;

    if (node->write != NULL)
        ret = node->write(node, offset, size, contents);
    else if (node->flags != FS_FOLDER) {
        node->contents = (char *)realloc(node->contents, strlen(contents) + 1);
        strcpy(node->contents, contents);
    } else {
        ret = 1;
    }

    node->size = strlen(contents);
    nodes[node->id] = node;

    return ret;
}

int read_node(fs_node_t *node, int offset, int size, char *buffer) {
    int ret = 0;

    if (node->read != NULL) {
        ret = node->read(node, offset, size, buffer);
    } else if (node->flags != FS_FOLDER) {
        set_string(buffer, node->contents);
    }

    return ret;
}

char *get_type(fs_node_t *node) {
    switch (node->flags) {
        case FS_FOLDER:
            return strdup("folder");

        case FS_FILE:
            return strdup("file");

        default:
            return strdup("unknown");
    }
}

int list_dir(fs_node_t *node, int index) {
    if (node == NULL)
        return 1;

    for (int z = 0; z < index; z++)
        printf("    ");

    for (int z = 0; z < index; z++)
        Kernel::serial_write_string("    ");

    char *type = get_type(node);

    printf("%s (%s)\n", node->name, type);
    Kernel::serial_write_string("%s (%s) (%s)\n", node->name, type, node->path);

    free(type);

    for (int z = 0; z < node->children_count; z++)
        list_dir(nodes[node->children[z]], index + 1);

    return 0;
}

int list_dir(char *dir) {
    fs_node_t *node = find_node(dir);

    if (node == NULL || node->flags == FS_FILE)
        return 1;

    int ret = list_dir(node, 0);

    return ret;
}

void create_root() {
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

void init_fs() {
    create_root();

    make_dir("dev", "/");
    make_dir("apps", "/");

    log::info("Initialized virtual file system");
}
