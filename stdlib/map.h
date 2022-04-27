#include <string.h>
#include <ctype.h>

namespace std {

template <typename T>
class map {
    typedef struct {
        char *name;
        T val;
    } MapKey;

    MapKey *keys;
    int mkey_count = 0;

    void add_key(MapKey key, int pos) {
        for (int z = mkey_count - 1; z > pos; z--) {
            keys[z + 1] = keys[z];
        }

        keys[pos] = key;
        mkey_count++;
    }

public:
    map() {}

    int size() {
        return mkey_count;
    }

    bool has_name(char *name) {
        for (int z = 0; z < mkey_count; z++) {
            if (strcmp(keys[z].name, name) == 0) {
                return true;
            }
        }

        return false;
    }

    int find_pos_of(char *name) {
        for (int z = 0; z < mkey_count; z++) {
            if (strcmp(keys[z].name, name) == 0) {
                return z;
            }
        }

        return -1;
    }

    char *find_name_of(int pos) {
        if (pos < 0 || pos >= mkey_count) {
            return NULL;
        }

        return keys[pos].name;
    }

    T find(char *name) {
        int pos = find_pos_of(name);

        if (pos >= 0) {
            return keys[pos].val;
        } else {
            return (T)NULL;
        }
    }

    T find(int pos) {
        char *name = NULL;

        if ((name = find_name_of(pos)) != NULL) {
            return find(name);
        } else {
            return NULL;
        }
    }

    void push_back(char *name, T val) {
        insert(name, val, mkey_count);
    }

    void insert(char *name, T val, int pos) {
        if (has_name(name)) {
            return;
        }

        MapKey key;
        key.name = name;
        key.val = val;
        add_key(key, pos);
    }

    void remove(char *name) {
        int pos = find_pos_of(name);

        if (pos >= 0) {
            for (int z = pos; z < mkey_count; z++) {
                keys[z] = keys[z + 1];
            }

            mkey_count--;
        }
    }

    void remove(int pos) {
        char *name = NULL;

        if ((name = find_name_of(pos)) != NULL) {
            remove(name);
        }
    }

    void pop() {
        if (mkey_count > 0) {
            remove(mkey_count - 1);
        }
    }

    void erase() {
        while (size() != 0) {
            pop();
        }
    }

    void merge_map(auto _map) {
        for (int z = 0; z < _map.size(); z++) {
            push_back(_map.find_name_of(z), _map.find(z));
        }
    }

    void replace(int z, T val) {
        if (z < 0 || z >= mkey_count) {
            return;
        }

        auto key = keys[z];
        key.val = val;
        keys[z] = key;
    }

    T operator[](int pos) {
        return find(pos);
    }

    T operator()(int pos) {
        return find(pos);
    }

    void operator+(auto _map) {
        merge_map(_map);
    }

    void operator+=(auto _map) {
        merge_map(_map);
    }

    void operator=(auto _map) {
        erase();
        merge_map(_map);
    }

    bool operator>(auto _map) {
        if (size() > _map.size()) {
            return true;
        }

        return false;
    }

    bool operator<(auto _map) {
        if (size() < _map.size()) {
            return true;
        }

        return false;
    }

    bool operator==(auto _map) {
        if (size() != _map.size()) {
            return false;
        }

        for (int z = 0; z < size(); z++) {
            if (strcmp(keys[z].name, _map.find_name_of(z)) != 0) {
                return false;
            }

            if (keys[z].val != _map.find(z)) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(auto _map) {
        if (size() != _map.size()) {
            return true;
        }

        for (int z = 0; z < size(); z++) {
            if (strcmp(keys[z].name, _map.find_name_of(z)) != 0) {
                return true;
            }

            if (keys[z].val != _map.find(z)) {
                return true;
            }
        }

        return false;
    }

    bool operator<=(auto _map) {
        if (size() <= _map.size()) {
            return true;
        }

        return false;
    }

    bool operator>=(auto _map) {
        if (size() >= _map.size()) {
            return true;
        }

        return false;
    }

    bool operator!() {
        if (size()) {
            return false;
        }

        return true;
    }

    ~map() {}
};

}
