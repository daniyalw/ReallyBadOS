#pragma once

#include <list.h>

namespace Utils {

class Tree {
private:
    void *_data;
    Tree *_parent;
    std::vector<Tree *> children;
public:

    Tree() {}
    Tree(void *data) { _data = data; }

    void add(Tree *tree);
    void set(void *data);
    void remove(int tree);
    void *data() { return _data; }
    Tree *child(int z) { return children.get(z); }
    int size() { return children.size(); }
    Tree *parent() { return _parent; }
    void parent(Tree *par) { _parent = par; }
};

}
