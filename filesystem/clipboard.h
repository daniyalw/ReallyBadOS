#pragma once

char _clipboard[100];

int internal_clipboard_read(fs_node_t *node, int offset, int size, char *buf);
int internal_clipboard_write(fs_node_t *node, int offset, int size, char *buf);
int init_clipboard();

char *clipboard_read();
int clipboard_write(char *data);
