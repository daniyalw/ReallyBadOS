#pragma once

char _clipboard[100];

int clipboard_read(fs_node_t *node, int offset, int size, char *buf);
int clipboard_write(fs_node_t *node, int offset, int size, char *buf);
int init_clipboard();
