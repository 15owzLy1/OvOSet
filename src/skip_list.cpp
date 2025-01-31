//
// Created by Lddyss on 2025/1/30.
//
#include "skip_list.h"

// 构造函数
template <typename Key, class Comparator>
SkipList<Key, Comparator>::SkipList(int maxLevel) : maxLevel(maxLevel), currentLevel(0) {
    header = new SkipListNode(Key(), maxLevel); // 初始化头节点
    srand(time(0)); // 初始化随机数种子
}

// 析构函数
template <typename Key, class Comparator>
SkipList<Key, Comparator>::~SkipList() {
    SkipListNode* current = header->forward[0];
    while (current) {
        SkipListNode* temp = current;
        current = current->forward[0];
        delete temp;
    }
    delete header;
}

// 随机生成节点的层数
template <typename Key, class Comparator>
int SkipList<Key, Comparator>::randomLevel() {
    static const unsigned int kBranching = 4;
    int level = 0;
    while (level < maxLevel && (rand() % kBranching == 0)) { // 1/4 的概率增加一层
        level++;
    }
    return level;
}

// 插入操作
template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::Insert(const Key& key) {
    std::vector<SkipListNode*> update(maxLevel + 1, nullptr); // 记录每一层的插入位置
    SkipListNode* current = header;

    // 从最高层开始查找插入位置
    for (int i = currentLevel; i >= 0; i--) {
        while (current->forward[i] && compare(current->forward[i]->value, key) < 0) {
            current = current->forward[i];
        }
        update[i] = current; // 记录每一层的插入位置
    }

    // 随机生成新节点的层数
    int newLevel = randomLevel();

    // 如果新节点的层数比当前跳表的层数高，更新跳表的层数
    if (newLevel > currentLevel) {
        for (int i = currentLevel + 1; i <= newLevel; i++) {
            update[i] = header; // 新层的插入位置是头节点
        }
        currentLevel = newLevel; // 更新当前层数
    }

    // 创建新节点
    auto* newNode = new SkipListNode(key, newLevel);

    // 更新每一层的指针
    for (int i = 0; i <= newLevel; i++) {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }

    return true;
}

// 查找操作
template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::Find(const Key& key) const {
    SkipListNode* current = header;

    // 从最高层开始查找
    for (int i = currentLevel; i >= 0; i--) {
        while (current->forward[i] && compare(current->forward[i]->value, key) < 0) {
            current = current->forward[i];
        }
    }

    // 检查是否找到目标值
    current = current->forward[0];
    return current && compare(current->value, key) == 0;
}

// 删除操作
template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::Remove(const Key& key) {
    std::vector<SkipListNode*> update(maxLevel + 1, nullptr); // 记录每一层的删除位置
    SkipListNode* current = header;

    // 从最高层开始查找删除位置
    for (int i = currentLevel; i >= 0; i--) {
        while (current->forward[i] && compare(current->forward[i]->value, key) < 0) {
            current = current->forward[i];
        }
        update[i] = current; // 记录每一层的删除位置
    }

    // 找到目标节点
    current = current->forward[0];
    if (current && compare(current->value, key) == 0) {
        // 更新每一层的指针
        for (int i = 0; i <= currentLevel; i++) {
            if (update[i]->forward[i] != current) {
                break;
            }
            update[i]->forward[i] = current->forward[i];
        }

        // 删除节点
        delete current;

        // 如果删除的节点是最高层的节点，更新跳表的层数
        while (currentLevel > 0 && header->forward[currentLevel] == nullptr) {
            currentLevel--;
        }

        return true;
    }

    return false;
}

// 打印跳表
template <typename Key, class Comparator>
void SkipList<Key, Comparator>::Print() const {
    for (int i = currentLevel; i >= 0; i--) {
        SkipListNode* current = header->forward[i];
        std::cout << "Level " << i << ": ";
        while (current) {
            std::cout << current->value << " ";
            current = current->forward[i];
        }
        std::cout << std::endl;
    }
}
