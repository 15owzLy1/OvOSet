//
// Created by Lddyss on 2025/1/30.
//

#ifndef CODE_SKIP_LIST_H
#define CODE_SKIP_LIST_H

#include "ovo_set.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class SkipList : public OvOSet {
private:
    struct Node {
        int key;
        std::vector<Node*> forward;
        Node(int k) : key(k) {}
    };

    int level;
    Node* head;
    float p;

    int randomLevel();
    Node* createNode(int key);
    void insertNode(int key);
    bool deleteNode(int key);
    Node* findNode(int key) const;

public:
    SkipList(int maxLevel = 16, float p_ = 0.5) : level(0), head(nullptr), p(p_) {
        head = createNode(-1);
        for (int i = 0; i < maxLevel; ++i) head->forward.push_back(nullptr);
    }
    ~SkipList();
    void Insert(int key) override;
    bool Remove(int key) override;
    bool Find(int key) const override;
};

#endif //CODE_SKIP_LIST_H
