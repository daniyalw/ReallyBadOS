typedef struct tree_t
{
    void * val;
    tree_t *parent;
    tree_t *children[10];
    int c_count = 0;
} tree_t;

tree_t * add_node(tree_t * tree, tree_t *sub);
tree_t * add_val(tree_t * tree, void *val);
