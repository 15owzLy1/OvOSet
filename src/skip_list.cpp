//
// Created by Lddyss on 2025/1/30.
//
#include "skip_list.h"

// 构造函数
template <typename Key, class Comparator>
SkipList<Key, Comparator>::SkipList(uint16_t max_level, uint16_t branch_num)
    : maxLevel_(max_level), branchNum_(branch_num), cur_level_(0) {
}

// 析构函数
template <typename Key, class Comparator>
SkipList<Key, Comparator>::~SkipList() {
    SkipListNode* current = header_->forward_[0];
    while (current) {
        SkipListNode* temp = current;
        current = current->forward_[0];
        delete temp;
    }
    delete header_;
}

// 随机生成节点的层数
template <typename Key, class Comparator>
int SkipList<Key, Comparator>::randomLevel() {
    static const unsigned int kBranching = 4;
    int level = 0;
    while (level < maxLevel_ && (rand_() % kBranching == 0)) { // 1/4 的概率增加一层
        level++;
    }
    return level;
}

// 插入操作
template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::Insert(const Key& key) {
    std::vector<SkipListNode*> update(maxLevel_ + 1, nullptr); // 记录每一层的插入位置
    SkipListNode* current = header_;

    // 从最高层开始查找插入位置
    for (int i = cur_level_; i >= 0; i--) {
        while (current->forward_[i] && compare_(current->forward_[i]->k_, key) < 0) {
            current = current->forward_[i];
        }
        update[i] = current; // 记录每一层的插入位置
    }

    // 随机生成新节点的层数
    int newLevel = randomLevel();

    // 如果新节点的层数比当前跳表的层数高，更新跳表的层数
    if (newLevel > cur_level_) {
        for (int i = cur_level_ + 1; i <= newLevel; i++) {
            update[i] = header_; // 新层的插入位置是头节点
        }
        cur_level_ = newLevel; // 更新当前层数
    }

    // 创建新节点
    auto newNode =
            reinterpret_cast<SkipListNode*>(malloc(sizeof(Key) + sizeof(SkipListNode*) * newLevel));

    // 更新每一层的指针
    for (int i = 0; i <= newLevel; i++) {
        newNode->forward_[i] = update[i]->forward_[i];
        update[i]->forward_[i] = newNode;
    }

    return true;
}

// 查找操作
template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::Contains(const Key& key) const {
    SkipListNode* current = header_;

    // 从最高层开始查找
    for (int i = cur_level_; i >= 0; i--) {
        while (current->forward_[i] && compare_(current->forward_[i]->k_, key) < 0) {
            current = current->forward_[i];
        }
    }

    // 检查是否找到目标值
    current = current->forward_[0];
    return current && compare_(current->k_, key) == 0;
}

// 删除操作
template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::Remove(const Key& key) {
    std::vector<SkipListNode*> update(maxLevel_ + 1, nullptr); // 记录每一层的删除位置
    SkipListNode* current = header_;

    // 从最高层开始查找删除位置
    for (int i = cur_level_; i >= 0; i--) {
        while (current->forward_[i] && compare_(current->forward_[i]->k_, key) < 0) {
            current = current->forward_[i];
        }
        update[i] = current; // 记录每一层的删除位置
    }

    // 找到目标节点
    current = current->forward_[0];
    if (current && compare_(current->k_, key) == 0) {
        // 更新每一层的指针
        for (int i = 0; i <= cur_level_; i++) {
            if (update[i]->forward_[i] != current) {
                break;
            }
            update[i]->forward_[i] = current->forward_[i];
        }

        // 删除节点
        delete current;

        // 如果删除的节点是最高层的节点，更新跳表的层数
        while (cur_level_ > 0 && header_->forward_[cur_level_] == nullptr) {
            cur_level_--;
        }

        return true;
    }

    return false;
}

// 打印跳表
template <typename Key, class Comparator>
void SkipList<Key, Comparator>::Print() const {
    for (int i = cur_level_; i >= 0; i--) {
        SkipListNode* current = header_->forward_[i];
        std::cout << "Level " << i << ": ";
        while (current) {
            std::cout << current->k_ << " ";
            current = current->forward_[i];
        }
        std::cout << std::endl;
    }
}
