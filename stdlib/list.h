#pragma once

#include <stdio.h>
#include <string.h>

namespace std {

template <typename T>
class list {
private:
    int length = 0;
    T * arr;

public:
    list()
    {
        arr = (T *)malloc(length);
    }

    bool operator==(list other)
    {
        int size = other.size();

        if (size != length)
            return false;

        for (int z = 0; z < size; z++)
        {
            if (this->get(z) != other.get(z))
                return false;
        }

        return true;
    }

    bool operator>(list other)
    {
        if (length > other.size())
            return true;

        return false;
    }

    bool operator<(list other)
    {
        if (length < other.size())
            return true;

        return false;
    }

    bool operator!=(list other)
    {
        int size = other.size();

        if (size != length)
            return true;

        for (int z = 0; z < size; z++)
        {
            if (this->get(z) != other.get(z))
                return true;
        }

        return false;
    }

    void operator=(list other)
    {
        for (int z = 0; z < length + 1; z++)
            this->pop();

        if (length != 0)
        {
            error("Could not copy lists.");
            return;
        }

        for (int z = 0; z < other.size(); z++)
            this->push_back(other.get(z));
    }

    T operator[](int index)
    {
        return this->get(index);
    }

    void operator=(T object)
    {
        for (int z = 0; z < length + 1; z++)
            this->pop();

        this->push_back(object);
    }

    void merge(list<T> other)
    {
        for (int z = 0; z < other.size(); z++)
            this->push_back(other[z]);
    }

    // Function that returns the number of
    // elements in array after pushing the data
    void push_back(T data)
    {
        arr = (T *)realloc(arr, length + 1);
        arr[length] = data;
        length++;
    }

    T get(int value)
    {
        if (value > length) {
            // caller tried to access illegal value
            return (T)NULL;
        } else {
            return arr[value];
        }
    }

    void remove(int value)
    {
        T newarr[length];
        int nz = 0;

        for (int z = 0; z < length; z++)
        {
            if (z == value)
                continue;

            newarr[nz] = arr[z];
            nz++;
        }
        length = nz;
        memcpy_int(newarr, arr, length);
        arr = (T *)realloc(arr, length);
    }

    void clear_all()
    {
        for (int z = 0; z < length + 1; z++)
            this->pop();
    }

    void pop()
    {
        length--;
        T newarr[length];
        int nz = 0;

        for (int z = 0; z < length; z++)
        {
            newarr[nz] = arr[z];
            nz++;
        }
        length = nz;
        memcpy_int((uint32_t *)&newarr, (uint32_t *)&arr, length);
        arr = (T *)realloc(arr, length); // free a bit of memory
    }

    T get_last()
    {
        return arr[length - 1];
    }

    int size() {
        return length;
    }

    void insert_at(T insert, int pos)
    {
        arr = (T *)realloc(arr, length + 1);
        if (pos > length)
            return;

        for (int z = length; z > (pos - 1); z--)
        {
            arr[z+1] = arr[z];
        }

        arr[pos] = insert;
        length++;
    }

    void move(int oldpos, int newpos)
    {
        if (oldpos > length || newpos > length)
            return;

        T copied = arr[oldpos];
        this->remove(oldpos);
        this->insert_at(copied, newpos);
    }

    int get_first_pos(T obj)
    {
        for (int z = 0; z < length; z++)
        {
            if (arr[z] == obj)
                return z;
        }

        return -1;
    }

    int get_last_pos(T obj)
    {
        for (int z = length; z > 0; z--)
        {
            if (arr[z] == obj)
                return z;
        }

        return -1;
    }

    int find(T obj)
    {
        return get_first_pos(obj);
    }

    void replace(T rplce, int pos)
    {
        if (pos > length)
            return;

        arr[pos] = rplce;
    }

    void swap(int index1, int index2)
    {
        T index3;
        index3 = arr[index1];
        arr[index1] = arr[index2];
        arr[index2] = index3;
    }

    T last()
    {
        return this->get(length-1);
    }

    T type() { return T(); }
};

}

#define foreach(child, _list) \
                auto child = _list[0]; \
                for (int z = 0; z < _list.size(); z++, child = _list[z])
