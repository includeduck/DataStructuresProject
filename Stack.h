#ifndef STACK_H
#define STACK_H

#include "containers.h"

template <typename T>
class SimpleStack {
private:
    DynamicArray<T> data;
public:
    SimpleStack() {}
    void push(const T &v) { data.pushBack(v); }
    T pop() { T v = data[data.size()-1]; data.popBack(); return v; }
    bool empty() const { return data.size() == 0; }
};

#endif // STACK_H
