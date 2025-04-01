//
// Created by Lddyss on 2025/1/30.
//
#ifndef RB_TREE_H
#define RB_TREE_H

#include "ovo_set.h"
#include <iostream>
#include <functional>

enum Color { RED, BLACK };

template <typename Key, class Comparator>
class RBTree : public OvOSet<Key, Comparator> {
private:
    struct Node {
        Key k_;
        Color color_;
        Node* left_;
        Node* right_;
        Node* parent_;

        Node(const Key& k, Color c = RED, Node* l = nullptr, Node* r = nullptr, Node* pa = nullptr)
                : k_(k), color_(c), left_(l), right_(r), parent_(pa) {}
    };

    Node* root_;
    Comparator compare_;
    std::mutex mutex_;

    void leftRotate(Node* x);
    void rightRotate(Node* x);
    void insertFixup(Node* z);
    void deleteFixup(Node* x);
    void transplant(Node* u, Node* v);
    bool findNode(const Key &k, Node** res) const ;
    bool insertNode(const Key &k, Node* &n_node);
    bool deleteNode(const Key &k);
    Node* minimum(Node* x);
    void destroy(Node* node);
    bool equal(const Key &i, const Key &j) const {
        return !compare_(i, j) && !compare_(j, i);
    }

public:
    RBTree();
    ~RBTree();

    bool Insert(const Key &key) override;
    bool Remove(const Key &key) override;
    bool Contains(const Key &key) override;
    void GetRange(const Key &lower, const Key &upper, std::vector<Key>* result) override;

};

template class RBTree<int, std::less<>>;
template class RBTree<uint32_t, std::less<>>;
template class RBTree<uint64_t, std::less<>>;

#endif // RB_TREE_H
