//
// Created by Lddyss on 2025/1/30.
//
#ifndef RB_TREE_H
#define RB_TREE_H

#include "ovo_set.h"
#include <iostream>

enum Color { RED, BLACK };

template <typename Key, class Comparator>
class RBTree : public OvOSet<Key, Comparator> {
private:
    struct Node {
        Key key;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(const Key& k, Color c = RED, Node* l = nullptr, Node* r = nullptr, Node* p = nullptr)
                : key(k), color(c), left(l), right(r), parent(p) {}
    };

    Node* root;
    Comparator compare;

    void rotateLeft(Node*& root, Node*& pt);
    void rotateRight(Node*& root, Node*& pt);
    void fixInsert(Node*& root, Node*& pt);
    void fixDelete(Node*& root, Node*& pt);
    Node* insertBST(Node* root, Node* pt);
    Node* searchTree(Node* root, const Key& key) const;
    Node* minValueNode(Node* node);
    void transplant(Node*& root, Node* u, Node* v);
    void clear(Node* node);

public:
    RBTree();
    ~RBTree();

    bool Insert(const Key& key) override;
    bool Remove(const Key& key) override;
    bool Find(const Key& key) const override;

};

template class RBTree<int, std::less<>>;

#endif // RB_TREE_H