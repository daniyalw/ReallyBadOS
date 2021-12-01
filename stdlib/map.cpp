#pragma once

namespace std {

template <typename T, typename VALUE>
class map {
private:
    struct map_array {
        T object;
        VALUE value;
    };

    std::list<map_array> arr;

public:
    map()
    {
    }

    bool operator==(map other)
    {
        int size = other.size();

        if (size != this->size())
            return false;

        for (int z = 0; z < size; z++)
        {
            if ((this->get_value(z) != other.get_value(z)) && (this->get_key(z) != other.get_key(z)))
                return false;
        }

        return true;
    }

    bool operator>(map other)
    {
        if (this->size() > other.size())
            return true;

        return false;
    }

    bool operator<(map other)
    {
        if (this->size() < other.size())
            return true;

        return false;
    }

    bool operator!=(map other)
    {
        int size = other.size();

        if (size != this->size())
            return true;

        for (int z = 0; z < size; z++)
        {
            if ((this->get_value(z) != other.get_value(z)) && (this->get_key(z) != other.get_key(z)))
                return true;
        }

        return false;
    }

    // Function that returns the number of
    // elements in array after pushing the data
    void push_back(T data, VALUE val)
    {
        map_array a;
        a.object = data;
        a.value = val;
        arr.push_back(a);
    }

    T get_key(int pos)
    {
        return arr.get(pos).object;
    }

    VALUE get_value(int pos)
    {
        return arr.get(pos).value;
    }

    void remove(int pos)
    {
        arr.remove(pos);
    }

    void pop()
    {
        arr.pop();
    }

    VALUE get_last_value()
    {
        return arr.get_last().value;
    }

    T get_last_key()
    {
        return arr.get_last().object;
    }

    // Function that return the size of vector
    int size() {
        return arr.size();
    }

};

}
