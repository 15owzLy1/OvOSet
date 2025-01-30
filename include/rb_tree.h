//
// Created by Lddyss on 2025/1/30.
//

#ifndef CODE_RB_TREE_H
#define CODE_RB_TREE_H

#include "ovo_set.h"
#include <iostream>
#include <vector>

class RBTree : public OvOSet {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;
        Node* parent;
        bool isRed;
        Node(int k, Node* p = nullptr) : key(k), left(nullptr), right(nullptr), parent(p), isRed(true) {}
    };

    Node* root;
    void leftRotate(Node* x);
    void rightRotate(Node* y);
    void fixViolation(Node* z);
    void insertFixup(Node* k);
    void removeFixup(Node* x);
    void transplant(Node* u, Node* v);
    void deleteNode(Node* root, int key);
    Node* minValue(Node* node);
    Node* search(Node* node, int key) const;

public:
    RBTree() : root(nullptr) {}
    ~RBTree();
    void Insert(int key) override;
    bool Remove(int key) override;
    bool Find(int key) const override;
};

#endif //CODE_RB_TREE_H
