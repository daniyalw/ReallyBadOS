#include <tree.h>

void Tree::add(Tree *tree) {
    children.push_back(tree);
}

void Tree::remove(int tree) {
    children.delete_data(tree);
}

void Tree::set(void *_data) {
    data = _data;
}
