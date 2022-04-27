#pragma once

int port_read(fs_node_t *node, int offset, int size, char *buffer);
int port_write(fs_node_t *node, int offset, int size, char *buffer);
int init_portfs();
