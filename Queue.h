#ifndef QUEUE_H
#define QUEUE_H

#include "containers.h"

template <typename T>
class CircularQueue {
private:
    T *buffer;
    int capacity;
    int head;
    int tail;
    int count;
    void ensureCapacity(int need) {
        if (capacity >= need) return;
        int newCap = capacity>0?capacity:16;
        while (newCap < need) newCap *= 2;
        T *nb = (T*)realloc(buffer, sizeof(T)*newCap);
        buffer = nb; capacity = newCap;
    }
public:
    CircularQueue(): buffer(nullptr), capacity(0), head(0), tail(0), count(0) {}
    ~CircularQueue() { if (buffer) free(buffer); }
    void enqueue(const T &v) { ensureCapacity(count+1); buffer[tail]=v; tail=(tail+1)%capacity; ++count; }
    bool isEmpty() const { return count==0; }
    T dequeue() { T v = buffer[head]; head=(head+1)%capacity; --count; return v; }
};

#endif // QUEUE_H
