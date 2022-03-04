#include <tree.h>

tree_t *create_tree(tree_t *parent, char *name)
{
    tree_t *tree = (tree_t *)malloc(sizeof(tree_t *));

    tree->name = (char *)malloc(20);
    strcpy(tree->name, name);
    tree->children_count = 0;
    tree->abc = 4;
    tree->parent = parent;
    tree->data_count = 0;

    if (parent != NULL)
        tree->id = parent->children_count;
    else
        tree->id = -1;

    if (parent != NULL)
    {
        parent->children[parent->children_count] = tree;
        parent->children_count++;
    }

    save_tree(parent);

    return tree;
}

void save_tree(tree_t *tree)
{
    if (tree == NULL)
        return;

    if (tree->parent == NULL)
        return;

    tree->parent->children[tree->id] = tree;
    save_tree(tree->parent);
}

void set_tree_name(tree_t *tree, char *name)
{
    strcpy(tree->name, name);
    save_tree(tree);
}

void add_data(tree_t *tree, const void *data)
{
    tree->data[tree->data_count] = (void *)data;
    tree->data_count++;

    save_tree(tree);
}
