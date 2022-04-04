#include "node.h"

int test_write(fs_node_t *node, int offset, int size, char *buf) {
    log::info("testfs: Writing '%s' into file '%s'\n", buf, node->path);
    return 0;
}

int test_read(fs_node_t *node, int offset, int size, char *buf) {
    log::info("testfs: Reading from file '%s'\n", node->path);
    strcpy(buf, "SUCCESS");
    return 0;
}

int test_mkfile(fs_node_t *node) {
    log::info("Creating file '%s'\n", node->path);
    return 0;
}

int test_mkdir(fs_node_t *node) {
    log::info("Creating new folder %s\n", node->path);
    return 0;
}

void test_install() {
    fs_node_t *node = mount_fs("test", "/", test_write, test_read, test_mkfile, test_mkdir, USER_PERMISSION);

    if (node == NULL)
        log::info("Failed to mount 'test' at '/'\n");
    else
        log::info("Successfully mounted '%s' at '%s'!\n", node->name, nodes[node->parent_id]->path);
}
