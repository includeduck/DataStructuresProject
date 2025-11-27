#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>

template <typename T>
class DynamicArray {
private:
    T *data;
    size_t sizeUsed;
    size_t capacityAllocated;
    void growIfNeeded(size_t minCapacity) {
        if (capacityAllocated >= minCapacity) return;
        size_t newCap = capacityAllocated > 0 ? capacityAllocated : 16;
        while (newCap < minCapacity) newCap *= 2;
        T *nd = (T*)realloc(data, newCap * sizeof(T));
        data = nd; capacityAllocated = newCap;
    }
public:
    DynamicArray(): data(nullptr), sizeUsed(0), capacityAllocated(0) {}
    ~DynamicArray() { if (data) free(data); }
    void pushBack(const T &value) { growIfNeeded(sizeUsed + 1); data[sizeUsed++] = value; }
    void popBack() { if (sizeUsed>0) --sizeUsed; }
    T &operator[](size_t idx) { return data[idx]; }
    const T &operator[](size_t idx) const { return data[idx]; }
    size_t size() const { return sizeUsed; }
    size_t capacity() const { return capacityAllocated; }
    void reserve(size_t n) { if (n > capacityAllocated) growIfNeeded(n); }
    void clear() { sizeUsed = 0; }
};

template <typename T>
class SinglyLinkedList {
public:
    struct Node { T value; Node *next; Node(const T &v): value(v), next(nullptr) {} };
    Node *head;
    SinglyLinkedList(): head(nullptr) {}
    ~SinglyLinkedList() { Node *cur = head; while (cur) { Node *n = cur->next; delete cur; cur = n; } }
    void pushFront(const T &v) { Node *n = new Node(v); n->next = head; head = n; }
    bool isEmpty() const { return head == nullptr; }
};

static unsigned long djb2Hash(const char *s) {
    unsigned long h = 5381; int c;
    while ((c = (unsigned char)*s++)) h = ((h << 5) + h) + c;
    return h;
}

class HashMapStringToInt {
private:
    struct Entry { char *key; int value; Entry *next; Entry(): key(nullptr), value(0), next(nullptr) {} };
    Entry **buckets;
    int bucketCount;
    static char *strdup_local(const char *s) {
        if (!s) return nullptr;
        size_t n = strlen(s);
        char *r = (char*)malloc(n+1);
        memcpy(r, s, n+1);
        return r;
    }
public:
    HashMapStringToInt(int capacity = 4096) { bucketCount = capacity>16?capacity:16; buckets = (Entry**)calloc(bucketCount, sizeof(Entry*)); }
    ~HashMapStringToInt() { for (int i=0;i<bucketCount;i++) { Entry *e = buckets[i]; while (e) { Entry *n = e->next; if (e->key) free(e->key); delete e; e = n; } } free(buckets); }
    int get(const char *k) const {
        if (!k) return -1;
        unsigned long h = djb2Hash(k) % bucketCount; Entry *e = buckets[h]; while (e) { if (strcmp(e->key, k)==0) return e->value; e = e->next; } return -1;
    }
    void put(const char *k, int v) {
        unsigned long h = djb2Hash(k) % bucketCount; Entry *e = buckets[h]; while (e) { if (strcmp(e->key, k)==0) { e->value = v; return; } e = e->next; }
        Entry *ne = new Entry(); ne->key = strdup_local(k); ne->value = v; ne->next = buckets[h]; buckets[h] = ne;
    }
    void increment(const char *k) {
        int cur = get(k); if (cur >= 0) put(k, cur + 1); else put(k, 1);
    }
    int getBucketCount() const { return bucketCount; }
    Entry **getBuckets() const { return buckets; }
};

template <typename T>
class MinHeapGeneric {
private:
    T **array;
    int cap;
    int len;
    double (*costAccessor)(const T*);
    void swap(int a, int b) { T *t=array[a]; array[a]=array[b]; array[b]=t; }
    void siftUp(int i) {
        while (i>0) {
            int p=(i-1)/2;
            if (costAccessor(array[p]) <= costAccessor(array[i])) break;
            swap(p,i); i=p;
        }
    }
    void siftDown(int i) {
        while (1) {
            int l=2*i+1, r=2*i+2, s=i;
            if (l < len && costAccessor(array[l]) < costAccessor(array[s])) s=l;
            if (r < len && costAccessor(array[r]) < costAccessor(array[s])) s=r;
            if (s==i) break;
            swap(i,s); i=s;
        }
    }
public:
    MinHeapGeneric(int initialCapacity, double (*accessor)(const T*)) { cap = initialCapacity>16?initialCapacity:16; array = (T**)malloc(sizeof(T*)*cap); len = 0; costAccessor = accessor; }
    ~MinHeapGeneric() { free(array); }
    void push(T *item) { if (len+1 > cap) { cap *= 2; array = (T**)realloc(array, sizeof(T*)*cap); } array[len]=item; siftUp(len); ++len; }
    T *pop() { if (len==0) return nullptr; T *r = array[0]; array[0]=array[--len]; if (len>0) siftDown(0); return r; }
    bool empty() const { return len==0; }
};

#endif // CONTAINERS_H
