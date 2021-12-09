#pragma once

#include <list.h>

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

    void operator=(map other)
    {
        for (int z = 0; z < arr.size() + 1; z++)
            arr.pop();

        if (arr.size() != 0)
        {
            error("Could not pop all values in map.");
            return;
        }

        for (int z = 0; z < other.size(); z++)
            this->push_back(other.get(z).object, other.get(z).value);
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

    void insert_at(T key, VALUE val, int pos)
    {
        map_array a;
        a.object = key;
        a.value = val;
        arr.insert_at(a, pos);
    }

    void replace(T newobject, VALUE newval, int pos)
    {
        map_array a;
        a.object = newobject;
        a.value = newval;
        arr.replace(a, pos);
    }

    int get_first_pos(T obj)
    {
        for (int z = 0; z < arr.size(); z++)
            if (arr.get(z).object == obj)
                return z;

        return NULL;
    }

    int get_last_pos(T obj)
    {
        for (int z = arr.size(); z > 0; z--)
            if (arr.get(z).object == obj)
                return z;

        return NULL;
    }


    void move(int oldpos, int newpos)
    {
        arr.move(oldpos, newpos);
    }

    void swap(int index1, int index2)
    {
        arr.swap(index1, index2);
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
