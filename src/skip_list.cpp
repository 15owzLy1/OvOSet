//
// Created by Lddyss on 2025/1/30.
//
#include "skip_list.h"

template <typename Key, class Comparator>
struct SkipList<Key, Comparator>::SkipListNode {
    explicit SkipListNode(const Key &k) : k_(k) {}
    explicit SkipListNode(Key &&k) : k_(std::move(k)) {}
    Key const k_;
    std::atomic<SkipListNode*> next_[0];
    static SkipListNode* NewNode(const Key &k, const int level) {
        auto node_memory = malloc(sizeof(Key) + sizeof(SkipListNode*) * level);
        return new (node_memory) SkipListNode(k);
    }
    static SkipListNode* NewNode(Key &&k, const int level) {
        auto node_memory = malloc(sizeof(Key) + sizeof(SkipListNode*) * level);
        return new (node_memory) SkipListNode(std::move(k));
    }
    SkipListNode* Next(int i) {
        assert(i >= 0);
        return next_[i].load(std::memory_order_acquire);
    }
    void SetNext(int i, SkipListNode* p) {
        assert(i >= 0);
        next_[i].store(p, std::memory_order_release);
    }
    SkipListNode* unsafe_Next(int i) {
        assert(i >= 0);
        return next_[i].load(std::memory_order_relaxed);
    }
    void unsafe_SetNext(int i, SkipListNode* p) {
        assert(i >= 0);
        next_[i].store(p, std::memory_order_relaxed);
    }
};

// 构造函数
template <typename Key, class Comparator>
SkipList<Key, Comparator>::SkipList(uint16_t max_level, uint16_t branch_num)
    : maxLevel_(max_level), branchNum_(branch_num), cur_level_(1), head_(SkipListNode::NewNode(0, maxLevel_)) {
    for (int i = 0; i < maxLevel_; ++i) {
        head_->SetNext(i, nullptr);
    }
}

// 析构函数
template <typename Key, class Comparator>
SkipList<Key, Comparator>::~SkipList() {
    SkipListNode* p = head_;
    while (p != nullptr) {
        SkipListNode* tmp = p;
        p = p->next_[0];
        delete tmp;
    }
}

template <typename Key, class Comparator>
int SkipList<Key, Comparator>::getRandomLevel() {
    int l = 1;
    while (l < maxLevel_ && (rand_() % branchNum_ == 0)) {
        l++;
    }
    this->memory_usage_ += l * sizeof(SkipListNode*) + sizeof(Key);
    assert(l > 0);
    return l;
}

template <typename Key, class Comparator>
SkipList<Key, Comparator>::SkipListNode*
SkipList<Key, Comparator>::upperBound(const Key &k, SkipListNode** prev) const {
    auto p = head_;
    auto l = getCurrentLevel() - 1;
    while (true) {
        auto next = p->Next(l);
        if (next != nullptr && compare_(next->k_, k)) {
            p = next;
        } else {
            if (prev != nullptr) {
                prev[l] = p;
            }
            if (l == 0) {
                return next;
            } else {
                --l;
            }
        }
    }
}

// 插入操作
template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::Insert(const Key& k) {
    SkipListNode* prev[maxLevel_];
    SkipListNode* current = head_;
    auto p = upperBound(k, prev);

    if (p != nullptr && equal(p->k_, k)) {
//        std::cerr << __func__ << ", SkipList cannot Insert duplicated key." << std::endl;
        return false;
    }

    auto level = getRandomLevel();
    if (level > getCurrentLevel()) {
        for (int i = getCurrentLevel(); i < level; ++i) {
            prev[i] = head_;
        }
        cur_level_.store(level, std::memory_order_release);
    }

    p = SkipListNode::NewNode(k, level);
    for (int i = 0; i < level; ++i) {
        p->unsafe_SetNext(i, prev[i]->unsafe_Next(i));
        prev[i]->SetNext(i, p);
    }

    return true;
}

// 查找操作
template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::Contains(const Key& k) const {
    auto p = upperBound(k, nullptr);
    return p != nullptr && equal(p->k_, k);
}

// 删除操作
template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::Remove(const Key& k) {
    SkipListNode* prev[maxLevel_];
    auto p = upperBound(k, prev);
    if (p == nullptr) {
        return false;
    }
    for (int i = getCurrentLevel() - 1; i >= 0; --i) {
        if (prev[i]->Next(i) == p) {
            prev[i]->SetNext(i, p->Next(i));
        }
    }
    delete p;
    return true;
}
#pragma clang diagnostic pop

// 打印跳表
template <typename Key, class Comparator>
void SkipList<Key, Comparator>::Print() const {
    for (int i = cur_level_; i >= 0; i--) {
        SkipListNode* p = head_->next_[i];
        std::cout << "Level " << i << ": ";
        while (p) {
            std::cout << p->k_ << " ";
            p = p->next_[i];
        }
        std::cout << std::endl;
    }
}
