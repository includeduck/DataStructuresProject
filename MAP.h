#pragma once
#include <iostream>
#include <algorithm>

template <typename Key, typename Value>
class Map
{
private:
    struct Node
    {
        Key key;
        Value value;
        Node* left;
        Node* right;
        int height;

        Node(const Key& k, const Value& v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    int height(Node* node)
    {
        return node ? node->height : 0;
    }

    void updateHeight(Node* node)
    {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }

    int balanceFactor(Node* node)
    {
        return node ? height(node->left) - height(node->right) : 0;
    }

    Node* rotateRight(Node* y)
    {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    Node* rotateLeft(Node* x)
    {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    Node* balance(Node* node)
    {
        int bf = balanceFactor(node);

        if (bf > 1)
        {
            if (balanceFactor(node->left) < 0) node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (bf < -1)
        {
            if (balanceFactor(node->right) > 0) node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }

    Node* insert(Node* node, const Key& key, const Value& value)
    {
        if (!node) return new Node(key, value);

        if (key < node->key) node->left = insert(node->left, key, value);
        else if (key > node->key) node->right = insert(node->right, key, value);
        else
        {
            node->value = value;
            return node;
        }

        updateHeight(node);
        return balance(node);
    }

    Node* minValueNode(Node* node)
    {
        while (node->left) node = node->left;
        return node;
    }

    Node* erase(Node* node, const Key& key)
    {
        if (!node) return nullptr;

        if (key < node->key) node->left = erase(node->left, key);
        else if (key > node->key) node->right = erase(node->right, key);
        else
        {
            if (!node->left || !node->right)
            {
                Node* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            }
            else
            {
                Node* temp = minValueNode(node->right);
                node->key = temp->key;
                node->value = temp->value;
                node->right = erase(node->right, temp->key);
            }
        }

        updateHeight(node);
        return balance(node);
    }

    Node* find(Node* node, const Key& key) const
    {
        while (node)
        {
            if (key < node->key) node = node->left;
            else if (key > node->key) node = node->right;
            else return node;
        }
        return nullptr;
    }

    void destroy(Node* node)
    {
        if (node)
        {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

    Node* copy(Node* other)
    {
        if (!other) return nullptr;
        Node* node = new Node(other->key, other->value);
        node->left = copy(other->left);
        node->right = copy(other->right);
        node->height = other->height;
        return node;
    }

public:
    Map() : root(nullptr) {}

    Map(const Map& other) : root(copy(other.root)) {}

    Map& operator=(const Map& other)
    {
        if (this != &other)
        {
            destroy(root);
            root = copy(other.root);
        }
        return *this;
    }

    ~Map()
    {
        destroy(root);
    }

    void insert(const Key& key, const Value& value)
    {
        root = insert(root, key, value);
    }

    bool erase(const Key& key)
    {
        if (!find(key)) return false;
        root = erase(root, key);
        return true;
    }

    Value* find(const Key& key)
    {
        Node* node = find(root, key);
        return node ? &node->value : nullptr;
    }

    const Value* find(const Key& key) const
    {
        Node* node = find(root, key);
        return node ? &node->value : nullptr;
    }

    void inOrder(Node* node) const
    {
        if (node)
        {
            inOrder(node->left);
            std::cout << node->key << ": " << node->value << std::endl;
            inOrder(node->right);
        }
    }

    void printInOrder() const
    {
        inOrder(root);
    }
};