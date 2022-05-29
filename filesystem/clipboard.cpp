#include "node.h"
#include "file.h"
#include "configs.h"
#include "clipboard.h"

int internal_clipboard_read(fs_node_t *node, int offset, int size, char *buf) {
    strncpy(buf, &_clipboard[offset], size);
    return 0;
}

int internal_clipboard_write(fs_node_t *node, int offset, int size, char *buf) {
    memset(_clipboard, 0, 100);
    strncpy(_clipboard, &buf[offset], size);
    return 0;
}

int init_clipboard() {
    return create_file("clipboard", "/configs", internal_clipboard_read, internal_clipboard_write);
}

char *clipboard_read() {
    char *buffer = (char *)malloc(100);
    memset(buffer, 0, 100);

    FILE *file = fopen("/configs/clipboard", "r");

    if (!file) {
        free(buffer);
        return NULL;
    }

    int ret = fread(buffer, 1, 100, file);

    if (ret) {
        fclose(file);
        free(buffer);
        return NULL;
    }

    fclose(file);

    return buffer;
}

int clipboard_write(char *data) {
    FILE *file = fopen("/configs/clipboard", "w");

    if (!file) {
        return 1;
    }

    int ret = fwrite(data, 1, strlen(data), file);

    fclose(file);

    return ret;
}
