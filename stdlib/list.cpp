#pragma once

namespace std {

template <typename T>
class List {
private:
    T * arr;

    int capacity;

    int length;

    bool exceptions_on;

    int exceptions;

    void newarr() {
        // create a new array
        // and fill it up with the already given values
        // in the original array
        T * narr;
        int newl = 0; // new length

        int b = 0;
        for (int z = 0; z < length; z++)
        {
            // we check if it is not equal to exceptions
            if ((z != exceptions && exceptions_on) || (!exceptions_on))
            {
                narr[b] = arr[z];
                b++;
                newl++;
            }
        }
        // the original array is now the new array
        arr = narr;

        // new capacity
        capacity = sizeof(arr);
    }

public:
    List()
    {
        // capacity is always sizeof arr
        capacity = sizeof(arr);
        length = 0;
    }

    bool operator==(List other)
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

    bool operator>(List other)
    {
        if (this->size() > other.size())
            return true;

        return false;
    }

    bool operator<(List other)
    {
        if (this->size() < other.size())
            return true;

        return false;
    }

    bool operator!=(List other)
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
        if (capacity + length >= capacity) {
            newarr();
        }
        arr[length] = data;
        length++;
    }

    T get(int value)
    {
        if (value > length) {
            // caller tried to access illegal value
            return 0;
        } else {
            return arr[value];
        }
    }

    void remove(int value)
    {
        // we set the exceptions value to the value the user wants to remove
        // with newarr(), it'll loop through the values of the old array, and
        // check if the value is not exceptions
        exceptions = value;
        // we do this so the newarr() won't check if it's called outside of remove()
        exceptions_on = true;
        newarr();
        exceptions_on = false;
    }

    void pop()
    {
        // since we don't check above the length
        // we don't actually remove the element
        //
        // we just ignore the element
        length--;
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
