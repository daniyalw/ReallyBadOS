#pragma once

#include <stdio.h>
#include <string.h>

namespace std {

    template <typename T> class vector
    {
        T *arr;
        int capacity;
        int current;

    public:
        vector() {
            arr = new T[1];
            capacity = 1;
            current = 0;
        }

        T operator[](int index) {
            return get(index);
        }

        void operator=(auto vec) {
            clear();
            merge(vec);
        }

        bool operator==(auto vec) {
            if (size() != vec.size())
                return false;

            for (int z = 0; z < vec.size(); z++)
                if (get(z) != vec.get(z))
                    return false;

            return true;
        }

        bool operator!=(auto vec) {
            if (vec.size() != size())
                return true;

            bool result = false;

            for (int z = 0; z < vec.size(); z++)
                if (get(z) != vec.get(z))
                    result = true;

            return result;
        }

        bool operator!() {
            if (current == 0)
                return true;

            return false;
        }

        bool operator<(auto vec) {
            if (size() < vec.size())
                return true;

            return false;
        }

        bool operator>(auto vec) {
            if (size() > vec.size())
                return true;

            return false;
        }

        void merge(auto vec) {
            for (int z = 0; z < vec.size(); z++)
                this->push(vec[z]);
        }

        void clear() {
            while (current != 0)
                pop();
        }

        void insert(T data, int pos) {
            if (current == capacity) {
                T* temp = new T[2 * capacity];

                for (int i = 0; i < capacity; i++) {
                    temp[i] = arr[i];
                }

                delete[] arr;
                capacity *= 2;
                arr = temp;
            }

            for (int z = current - 1; z >= pos; z--)
                arr[z + 1] = arr[z];

            arr[pos] = data;
            current++;
        }

        void delete_data(int pos) {
            for (int z = pos + 1; z < current; z++)
                arr[z - 1] = arr[z];

            current--;
        }

        void push(T data) {
            if (current == capacity) {
                T* temp = new T[2 * capacity];

                for (int i = 0; i < capacity; i++) {
                    temp[i] = arr[i];
                }

                delete[] arr;
                capacity *= 2;
                arr = temp;
            }

            arr[current] = data;
            current++;
        }

        void replace(T data, int index) {
            if (index == capacity)
                push(data);
            else
                arr[index] = data;
        }

        void push_back(T data) {
            push(data);
        }

        T get(int index) {
            if (index < current && index >= 0)
                return arr[index];

            return (T)NULL;
        }

        void pop() {
            current--;
        }

        // function to get size of the vector
        int size() {
            return current;
        }

        // function to get capacity of the vector
        int getcapacity() {
            return capacity;
        }
    };
}
