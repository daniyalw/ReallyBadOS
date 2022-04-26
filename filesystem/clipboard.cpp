#include "node.h"
#include "file.h"
#include "configs.h"
#include "clipboard.h"

int clipboard_read(fs_node_t *node, int offset, int size, char *buf) {
    strncpy(buf, &_clipboard[offset], size);
    return 0;
}

int clipboard_write(fs_node_t *node, int offset, int size, char *buf) {
    memset(_clipboard, 0, 100);
    strncpy(_clipboard, &buf[offset], size);
    return 0;
}

int init_clipboard() {
    return create_file("clipboard", "/configs", clipboard_read, clipboard_write);
}
