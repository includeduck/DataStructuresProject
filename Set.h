#ifndef SET_H
#define SET_H

#include "Vector.h"

template<typename T>
class Set {
private:
    Vector<T> elements;

    int findIndex(const T& value) const {
        for (size_t i = 0; i < elements.size(); ++i) {
            if (elements[i] == value) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

public:
    Set() = default;

    // Insert an element into the set
    void insert(const T& value) {
        if (!contains(value)) {
            elements.push_back(value);
        }
    }

    // Remove an element from the set
    void erase(const T& value) {
        int index = findIndex(value);
        if (index != -1) {
            // Swap with last element and pop
            elements[index] = elements[elements.size() - 1];
            elements.pop_back();
        }
    }

    // Check if an element exists in the set
    bool contains(const T& value) const {
        return findIndex(value) != -1;
    }

    // Get the size of the set
    size_t size() const {
        return elements.size();
    }

    // Check if the set is empty
    bool empty() const {
        return elements.empty();
    }

    // Clear all elements from the set
    void clear() {
        elements.clear();
    }

    // Get all elements as a vector
    Vector<T> getElements() const {
        return elements;
    }

    // Union of two sets
    Set<T> unionWith(const Set<T>& other) const {
        Set<T> result = *this;
        Vector<T> otherElements = other.getElements();
        for (size_t i = 0; i < otherElements.size(); ++i) {
            result.insert(otherElements[i]);
        }
        return result;
    }

    // Intersection of two sets
    Set<T> intersectionWith(const Set<T>& other) const {
        Set<T> result;
        Vector<T> myElements = getElements();
        for (size_t i = 0; i < myElements.size(); ++i) {
            if (other.contains(myElements[i])) {
                result.insert(myElements[i]);
            }
        }
        return result;
    }

    // Difference of two sets (this - other)
    Set<T> differenceWith(const Set<T>& other) const {
        Set<T> result;
        Vector<T> myElements = getElements();
        for (size_t i = 0; i < myElements.size(); ++i) {
            if (!other.contains(myElements[i])) {
                result.insert(myElements[i]);
            }
        }
        return result;
    }

    // Check if this set is a subset of another set
    bool isSubsetOf(const Set<T>& other) const {
        Vector<T> myElements = getElements();
        for (size_t i = 0; i < myElements.size(); ++i) {
            if (!other.contains(myElements[i])) {
                return false;
            }
        }
        return true;
    }

    // Iterator support
    class Iterator {
    private:
        typename Vector<T>::Iterator it;

    public:
        Iterator(const typename Vector<T>::Iterator& iterator) : it(iterator) {}

        T& operator*() { return *it; }
        Iterator& operator++() {
            ++it;
            return *this;
        }
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator!=(const Iterator& other) const { return it != other.it; }
        bool operator==(const Iterator& other) const { return it == other.it; }
    };

    Iterator begin() {
        return Iterator(elements.begin());
    }

    Iterator end() {
        return Iterator(elements.end());
    }

    // Const iterator
    class ConstIterator {
    private:
        typename Vector<T>::ConstIterator it;

    public:
        ConstIterator(const typename Vector<T>::ConstIterator& iterator) : it(iterator) {}

        const T& operator*() const { return *it; }
        ConstIterator& operator++() {
            ++it;
            return *this;
        }
        ConstIterator operator++(int) {
            ConstIterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator!=(const ConstIterator& other) const { return it != other.it; }
        bool operator==(const ConstIterator& other) const { return it == other.it; }
    };

    ConstIterator begin() const {
        return ConstIterator(elements.begin());
    }

    ConstIterator end() const {
        return ConstIterator(elements.end());
    }
};

#endif