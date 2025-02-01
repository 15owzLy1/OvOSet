//
// Created by Lddyss on 2025/1/30.
//

#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "random.h"

template <typename Key, class Comparator>
class SkipList {
private:
    struct SkipListNode {
        Key k_;
        SkipListNode* forward_[1];
    };

    const uint16_t maxLevel_;
    const uint16_t branchNum_;
    Comparator compare_; // 比较器
    uint16_t cur_level_;   // 当前跳表的层数
    SkipListNode* header_; // 头节点
    Random rand_;

    // 随机生成节点的层数
    int randomLevel();

public:
    // 构造函数
    SkipList(uint16_t max_level = 16, uint16_t branch_num = 4);

    // 析构函数
    ~SkipList();

    // 插入操作
    bool Insert(const Key& key);

    // 查找操作
    bool Contains(const Key& key) const;

    // 删除操作
    bool Remove(const Key& key);

    // 打印跳表
    void Print() const;
};

template class SkipList<int, std::less<>>;

#endif // SKIP_LIST_H
