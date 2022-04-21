#include "node.h"
#include "file.h"
#include "null.h"

int null_fs_write(fs_node_t *node, int offset, int size, char *buf) {
    return 0;
}

int null_fs_read(fs_node_t *node, int offset, int size, char *buf) {
    return 0;
}

int init_null_fs() {
    return create_file("null", "/dev", null_fs_read, null_fs_write);
}
