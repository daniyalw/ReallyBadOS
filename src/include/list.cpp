template <typename T>
class List {
private:
    T * arr;

    int capacity;

    int length;

    void newarr() {
        T * narr;
        int b = 0;
        for (int z = 0; z < length; z++)
        {
            narr[b] = arr[z];
            b++;
        }
        arr = narr;
    }

public:
    List()
    {
        capacity = sizeof(arr);
        length = 0;
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
            return 0;
        } else {
            return arr[value];
        }
    }

    void pop()
    {
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
