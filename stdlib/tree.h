#pragma once

typedef struct tree_t
{
    int abc;
    char *name;
    int children_count;
    int id;
    void *data[10];
    int data_count;

    tree_t *children[10];
    tree_t *parent;
} tree_t;

tree_t *create_tree(tree_t *parent, char *name);
void save_tree(tree_t *tree);
void set_tree_name(tree_t *tree, char *name);
void add_data(tree_t *tree, const void *data);
