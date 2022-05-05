#include <tree.h>

namespace Utils {

void Tree::add(Tree *tree) {
    children.push_back(tree);
    tree->parent(this);
}

void Tree::remove(int tree) {
    children.delete_data(tree);
}

void Tree::set(void *data) {
    _data = data;
}

}
