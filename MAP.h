#ifndef MAP_H
#define MAP_H

#include "Vector.h"
#include <utility>
#include <stdexcept>
#include <functional>

template<typename Key, typename Value>
class Map {
private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        int height;

        Node(const Key& k, const Value& v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;
    size_t mapSize;

    int height(Node* node) const {
        return node ? node->height : 0;
    }

    int balanceFactor(Node* node) const {
        return node ? height(node->left) - height(node->right) : 0;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;

        return y;
    }

    Node* insert(Node* node, const Key& key, const Value& value) {
        if (!node) {
            ++mapSize;
            return new Node(key, value);
        }

        if (key < node->key) {
            node->left = insert(node->left, key, value);
        }
        else if (key > node->key) {
            node->right = insert(node->right, key, value);
        }
        else {
            node->value = value; // Update existing key
            return node;
        }

        node->height = 1 + std::max(height(node->left), height(node->right));

        int balance = balanceFactor(node);

        // Left Left Case
        if (balance > 1 && key < node->left->key) {
            return rotateRight(node);
        }

        // Right Right Case
        if (balance < -1 && key > node->right->key) {
            return rotateLeft(node);
        }

        // Left Right Case
        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* find(Node* node, const Key& key) const {
        if (!node || node->key == key) return node;

        if (key < node->key) return find(node->left, key);
        return find(node->right, key);
    }

    void inOrderTraversal(Node* node, Vector<Key>& keys) const {
        if (node) {
            inOrderTraversal(node->left, keys);
            keys.push_back(node->key);
            inOrderTraversal(node->right, keys);
        }
    }

    void inOrderTraversalPairs(Node* node, Vector<std::pair<Key, Value>>& pairs) const {
        if (node) {
            inOrderTraversalPairs(node->left, pairs);
            pairs.push_back(std::make_pair(node->key, node->value));
            inOrderTraversalPairs(node->right, pairs);
        }
    }

    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    Node* copyTree(Node* other) {
        if (!other) return nullptr;

        Node* newNode = new Node(other->key, other->value);
        newNode->left = copyTree(other->left);
        newNode->right = copyTree(other->right);
        newNode->height = other->height;
        return newNode;
    }

public:
    Map() : root(nullptr), mapSize(0) {}

    Map(const Map& other) : root(copyTree(other.root)), mapSize(other.mapSize) {}

    Map& operator=(const Map& other) {
        if (this != &other) {
            clear(root);
            root = copyTree(other.root);
            mapSize = other.mapSize;
        }
        return *this;
    }

    ~Map() {
        clear(root);
    }

    void insert(const Key& key, const Value& value) {
        root = insert(root, key, value);
    }

    bool contains(const Key& key) const {
        return find(root, key) != nullptr;
    }

    // Non-const operator[] for assignment
    Value& operator[](const Key& key) {
        Node* node = find(root, key);
        if (!node) {
            insert(key, Value());
            node = find(root, key);
        }
        return node->value;
    }

    // Const operator[] for read-only access
    const Value& operator[](const Key& key) const {
        Node* node = find(root, key);
        if (!node) throw std::out_of_range("Key not found");
        return node->value;
    }

    Value& at(const Key& key) {
        Node* node = find(root, key);
        if (!node) throw std::out_of_range("Key not found");
        return node->value;
    }

    const Value& at(const Key& key) const {
        Node* node = find(root, key);
        if (!node) throw std::out_of_range("Key not found");
        return node->value;
    }

    Vector<Key> keys() const {
        Vector<Key> result;
        inOrderTraversal(root, result);
        return result;
    }

    Vector<std::pair<Key, Value>> items() const {
        Vector<std::pair<Key, Value>> result;
        inOrderTraversalPairs(root, result);
        return result;
    }

    size_t size() const {
        return mapSize;
    }

    bool empty() const {
        return mapSize == 0;
    }

    void clear() {
        clear(root);
        root = nullptr;
        mapSize = 0;
    }
};

#endif