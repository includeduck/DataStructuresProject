#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdexcept>

template<typename T>
struct ListNode {
    T data;
    ListNode* next;
    ListNode* prev;

    ListNode(const T& val) : data(val), next(nullptr), prev(nullptr) {}
};

template<typename T>
class LinkedList {
private:
    ListNode<T>* head;
    ListNode<T>* tail;
    size_t listSize;

public:
    LinkedList() : head(nullptr), tail(nullptr), listSize(0) {}

    ~LinkedList() {
        clear();
    }

    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), listSize(0) {
        ListNode<T>* current = other.head;
        while (current) {
            push_back(current->data);
            current = current->next;
        }
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            ListNode<T>* current = other.head;
            while (current) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    void push_back(const T& value) {
        ListNode<T>* newNode = new ListNode<T>(value);
        if (!head) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        ++listSize;
    }

    void push_front(const T& value) {
        ListNode<T>* newNode = new ListNode<T>(value);
        if (!head) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        ++listSize;
    }

    void pop_back() {
        if (!tail) return;

        ListNode<T>* temp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;

        delete temp;
        --listSize;
    }

    void pop_front() {
        if (!head) return;

        ListNode<T>* temp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;

        delete temp;
        --listSize;
    }

    T& front() {
        if (!head) throw std::runtime_error("List is empty");
        return head->data;
    }

    T& back() {
        if (!tail) throw std::runtime_error("List is empty");
        return tail->data;
    }

    const T& front() const {
        if (!head) throw std::runtime_error("List is empty");
        return head->data;
    }

    const T& back() const {
        if (!tail) throw std::runtime_error("List is empty");
        return tail->data;
    }

    size_t size() const {
        return listSize;
    }

    bool empty() const {
        return listSize == 0;
    }

    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    // Iterator support
    class Iterator {
    private:
        ListNode<T>* current;

    public:
        Iterator(ListNode<T>* node) : current(node) {}

        T& operator*() { return current->data; }
        T* operator->() { return &current->data; }
        Iterator& operator++() {
            current = current->next;
            return *this;
        }
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator!=(const Iterator& other) const { return current != other.current; }
        bool operator==(const Iterator& other) const { return current == other.current; }
    };

    // ConstIterator support
    class ConstIterator {
    private:
        const ListNode<T>* current;

    public:
        ConstIterator(const ListNode<T>* node) : current(node) {}

        const T& operator*() const { return current->data; }
        const T* operator->() const { return &current->data; }
        ConstIterator& operator++() {
            current = current->next;
            return *this;
        }
        ConstIterator operator++(int) {
            ConstIterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator!=(const ConstIterator& other) const { return current != other.current; }
        bool operator==(const ConstIterator& other) const { return current == other.current; }
    };

    // Non-const iterators
    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    // Const iterators
    ConstIterator begin() const { return ConstIterator(head); }
    ConstIterator end() const { return ConstIterator(nullptr); }

    // Const iterators for C++11 compatibility
    ConstIterator cbegin() const { return ConstIterator(head); }
    ConstIterator cend() const { return ConstIterator(nullptr); }
};

#endif