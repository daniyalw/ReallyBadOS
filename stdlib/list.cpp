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

        if (size != this->size())
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
        if (this->size() > other.size())
            return true;

        return false;
    }

    bool operator<(list other)
    {
        if (this->size() < other.size())
            return true;

        return false;
    }

    bool operator!=(list other)
    {
        int size = other.size();

        if (size != this->size())
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
        // since we don't check above the length
        // we don't actually remove the element
        //
        // we just ignore the element
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

    // Function that return the size of vector
    int size() {
        return length;
    }

};

}
