#include <tree.h>

tree_t * add_node(tree_t * tree, tree_t *sub)
{
    sub->parent = tree;
    tree->children[tree->c_count] = sub;
    sub->parent = tree;
    tree->c_count++;

    return tree;
}

tree_t * add_val(tree_t * tree, void *val)
{
    tree->val = val;

    return tree;
}
