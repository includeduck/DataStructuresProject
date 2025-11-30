#ifndef STACK_H
#define STACK_H

#include "LinkedList.h"

template<typename T>
class Stack {
private:
    LinkedList<T> list;

public:
    Stack() = default;

    void push(const T& value) {
        list.push_back(value);
    }

    void pop() {
        list.pop_back();
    }

    T& top() {
        return list.back();
    }

    const T& top() const {
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