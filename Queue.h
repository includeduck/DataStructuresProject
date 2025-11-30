#ifndef QUEUE_H
#define QUEUE_H

#include "LinkedList.h"

template<typename T>
class Queue {
private:
    LinkedList<T> list;

public:
    Queue() = default;

    void push(const T& value) {
        list.push_back(value);
    }

    void pop() {
        list.pop_front();
    }
    
    T& front() {
        return list.front();
    }

    const T& front() const {
        return list.front();
    }

    T& back() {
        return list.back();
    }

    const T& back() const {
        return list.back();
    }

    size_t size() const {
        return list.size();
    }

    bool empty() const {
        return list.empty();
    }
};

#endif