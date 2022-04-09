#pragma once

#include <list.h>

class Tree {
private:
    void *data;
    std::vector<Tree *> children;
public:

    Tree() {}
    Tree(void *_data) { data = _data; }

    void add(Tree *tree);
    void set(void *_data);
    void remove(int tree);
    void *get() { return data; }
    Tree *get(int z) { return children.get(z); }
    int size() { return children.size(); }
};
