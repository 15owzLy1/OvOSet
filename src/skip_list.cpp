//
// Created by Lddyss on 2025/1/30.
//
#include "skip_list.h"

int SkipList::randomLevel() {
    int lvl = 0;
    while (lvl < level && (rand() / (RAND_MAX + 1.0)) < p) ++lvl;
    return lvl;
}

SkipList::Node* SkipList::createNode(int key) {
    return new Node(key);
}

void SkipList::insertNode(int key) {
    Node* update[level + 1];
    Node* x = head;
    for (int i = level; i >= 0; --i) {
        while (x->forward[i] && x->forward[i]->key < key) x = x->forward[i];
        update[i] = x;
    }
    x = x->forward[0];
    if (!x || x->key != key) {
        int lvl = randomLevel();
        if (lvl > level) {
            for (int i = level + 1; i > lvl; --i) update[i] = head;
            level = lvl;
        }
        Node* node = createNode(key);
        for (int i = 0; i <= lvl; ++i) {
            node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = node;
        }
    }
}

bool SkipList::deleteNode(int key) {
    Node* update[level + 1];
    Node* x = head;
    for (int i = level; i >= 0; --i) {
        while (x->forward[i] && x->forward[i]->key < key) x = x->forward[i];
        update[i] = x;
    }
    x = x->forward[0];
    if (x && x->key == key) {
        for (int i = 0; i <= level && update[i]->forward[i] == x; ++i) {
            update[i]->forward[i] = x->forward[i];
        }
        while (level > 0 && !head->forward[level]) --level;
        delete x;
        return true;
    }
    return false;
}

SkipList::Node* SkipList::findNode(int key) const {
    Node* x = head;
    for (int i = level; i >= 0; --i) {
        while (x->forward[i] && x->forward[i]->key < key) x = x->forward[i];
    }
    x = x->forward[0];
    if (x && x->key == key) return x;
    return nullptr;
}

void SkipList::Insert(int key) {
    insertNode(key);
}
bool SkipList::Remove(int key) {
    return deleteNode(key);
}

bool SkipList::Find(int key) const {
    return findNode(key) != nullptr;
}

SkipList::~SkipList() {
    // Implement destructor to delete all nodes
}
