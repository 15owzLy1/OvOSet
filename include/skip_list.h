//
// Created by Lddyss on 2025/1/30.
//

#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

template <typename Key, class Comparator>
class SkipList {
private:
    struct SkipListNode {
        Key value;
        std::vector<SkipListNode*> forward; // 指向下一层节点的指针数组

        SkipListNode(const Key& val, int level) : value(val), forward(level + 1, nullptr) {}
    };

    int maxLevel;       // 跳表的最大层数
    int currentLevel;   // 当前跳表的层数
    SkipListNode* header; // 头节点
    Comparator compare; // 比较器

    // 随机生成节点的层数
    int randomLevel();

public:
    // 构造函数
    SkipList(int maxLevel = 16);

    // 析构函数
    ~SkipList();

    // 插入操作
    bool Insert(const Key& key);

    // 查找操作
    bool Find(const Key& key) const;

    // 删除操作
    bool Remove(const Key& key);

    // 打印跳表
    void Print() const;
};

template class SkipList<int, std::less<>>;

#endif // SKIP_LIST_H
