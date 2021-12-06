#pragma once

namespace std {

template <typename T>
class list {
private:
    T * arr;
    int length = 0;

    void newarr() {
        // create a new array
        // and fill it up with the already given values
        // in the original array
        T * narr;
        int newl = 0; // new length

        int b = 0;
        for (int z = 0; z < length; z++)
        {
            narr[b] = arr[z];
            b++;
            newl++;
        }
        // the original array is now the new array
        arr = narr;

    }

public:

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

    // Function that returns the number of
    // elements in array after pushing the data
    void push_back(T data)
    {
        arr[length] = data;
        length++;
    }

    T get(int value)
    {
        if (value > length) {
            // caller tried to access illegal value
            return arr[0];
        } else {
            return arr[value];
        }
    }

    void remove(int value)
    {
        T * newarr;
        int nz = 0;

        for (int z = 0; z < length; z++)
        {
            if (z == value)
                continue;

            newarr[nz] = arr[z];
            nz++;
        }
        length = nz;
        arr = newarr;
    }

    void pop()
    {
        length--;
        T * newarr;
        int nz = 0;

        for (int z = 0; z < length; z++)
        {
            newarr[nz] = arr[z];
            nz++;
        }
        length = nz;
        arr = newarr;
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
    }

    int get_last_pos(T obj)
    {
        for (int z = length; z > 0; z--)
        {
            if (arr[z] == obj)
                return z;
        }
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

};

}
