#pragma once

int null_fs_write(fs_node_t *node, int offset, int size, char *buf);
int null_fs_read(fs_node_t *node, int offset, int size, char *buf);
int init_null_fs();
