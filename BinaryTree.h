#ifndef BINARYTREE_H
#define BINARYTREE_H

template <typename T>
class BinaryTreeNode {
public:
    T value;
    BinaryTreeNode *left;
    BinaryTreeNode *right;
    BinaryTreeNode(const T &v): value(v), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinaryTree {
public:
    BinaryTreeNode<T> *root;
    BinaryTree(): root(nullptr) {}
    ~BinaryTree() { clear(root); }
    void clear(BinaryTreeNode<T> *n) { if (!n) return; clear(n->left); clear(n->right); delete n; }
};

#endif // BINARYTREE_H
