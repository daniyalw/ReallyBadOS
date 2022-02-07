#include <tree.h>

tree_t * add_node(tree_t * tree, tree_t *sub)
{
    sub->parent = tree;
    tree->children[tree->c_count] = sub;
    sub->parent = tree;
    tree->children[tree->c_count] = sub;
    tree->c_count++;

    return tree;
}

tree_t * add_val(tree_t * tree, const char *val)
{
    for (int z = 0; z < len((char *)val); z++)
        tree->val[z] = val[z];

    return tree;
}

tree_t * create_tree()
{
    tree_t *tree = (tree_t *)malloc(sizeof(tree_t *));

    for (int z = 0; z < 10; z++)
        tree->children[z] = NULL;

    for (int z = 0; z < 100; z++)
        tree->val[z] = 0;

    tree->parent = NULL;

    return tree;
}
