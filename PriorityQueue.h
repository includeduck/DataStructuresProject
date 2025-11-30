#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "Vector.h"
#include <functional>

template<typename T, typename Compare = std::less<T>>
class PriorityQueue {
private:
    Vector<T> heap;
    Compare comp;

    void heapifyUp(int index) {
        if (index == 0) return;

        int parent = (index - 1) / 2;
        if (comp(heap[parent], heap[index])) {
            std::swap(heap[parent], heap[index]);
            heapifyUp(parent);
        }
    }

    void heapifyDown(int index) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int extreme = index;

        if (left < heap.size() && comp(heap[extreme], heap[left])) {
            extreme = left;
        }

        if (right < heap.size() && comp(heap[extreme], heap[right])) {
            extreme = right;
        }

        if (extreme != index) {
            std::swap(heap[index], heap[extreme]);
            heapifyDown(extreme);
        }
    }

public:
    PriorityQueue() = default;

    explicit PriorityQueue(const Compare& compare) : comp(compare) {}

    void push(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    void pop() {
        if (heap.empty()) return;

        heap[0] = heap[heap.size() - 1];
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
    }

    T& top() {
        return heap[0];
    }

    const T& top() const {
        return heap[0];
    }

    size_t size() const {
        return heap.size();
    }

    bool empty() const {
        return heap.empty();
    }
};

#endif