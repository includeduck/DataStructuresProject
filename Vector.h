#ifndef VECTOR_H
#define VECTOR_H

#include <stdexcept>

template<typename T>
class Vector {
private:
    T* data;
    size_t capacity;
    size_t currentSize;

    void resize(size_t newCapacity) {
        if (newCapacity == 0) newCapacity = 1;

        T* newData = new T[newCapacity];
        for (size_t i = 0; i < currentSize && i < newCapacity; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
        if (currentSize > capacity) {
            currentSize = capacity;
        }
    }

public:
    Vector() : data(new T[1]), capacity(1), currentSize(0) {}

    Vector(size_t initialSize) : data(new T[initialSize > 0 ? initialSize : 1]),
        capacity(initialSize > 0 ? initialSize : 1),
        currentSize(initialSize) {
    }

    Vector(const Vector& other) : data(new T[other.capacity]), capacity(other.capacity), currentSize(other.currentSize) {
        for (size_t i = 0; i < currentSize; ++i) {
            data[i] = other.data[i];
        }
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            data = new T[other.capacity];
            capacity = other.capacity;
            currentSize = other.currentSize;
            for (size_t i = 0; i < currentSize; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~Vector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (currentSize == capacity) {
            resize(capacity * 2);
        }
        if (currentSize < capacity) {
            data[currentSize++] = value;
        }
    }

    void pop_back() {
        if (currentSize > 0) {
            --currentSize;
        }
    }

    T& operator[](size_t index) {
        if (index >= currentSize) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= currentSize) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    T& at(size_t index) {
        return (*this)[index];
    }

    const T& at(size_t index) const {
        return (*this)[index];
    }

    size_t size() const {
        return currentSize;
    }

    bool empty() const {
        return currentSize == 0;
    }

    void clear() {
        currentSize = 0;
    }

    T* begin() { return data; }
    T* end() { return data + currentSize; }
    const T* begin() const { return data; }
    const T* end() const { return data + currentSize; }
};

#endif