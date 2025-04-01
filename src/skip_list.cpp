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

template <typename Key, class Comparator>
SkipList<Key, Comparator>::SkipList(uint16_t max_level, uint16_t branch_num)
    : OvOSet<Key, Comparator>(), maxLevel_(max_level), branchNum_(branch_num),
      cur_level_(1), head_(SkipListNode::NewNode(Key(), maxLevel_)) {
    for (int i = 0; i < maxLevel_; ++i) {
        head_->SetNext(i, nullptr);
    }
}

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
SkipList<Key, Comparator>::lowerBound(const Key &k, SkipListNode** prev) const {
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

template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::Insert(const Key& k) {
    SkipListNode* prev[maxLevel_];
    SkipListNode* current = head_;
    auto p = lowerBound(k, prev);

    if (p != nullptr && equal(p->k_, k)) {
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

template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::Contains(const Key& k) {
    auto p = lowerBound(k, nullptr);
    return p != nullptr && equal(p->k_, k);
}

template <typename Key, class Comparator>
bool SkipList<Key, Comparator>::Remove(const Key& k) {
    SkipListNode* prev[maxLevel_];
    auto p = lowerBound(k, prev);
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

template <typename Key, class Comparator>
void SkipList<Key, Comparator>::GetRange(const Key &lower, const Key &upper, std::vector<Key>* result) {
    auto p = lowerBound(lower, nullptr);
    while (p != nullptr && compare_(p->k_, upper)) {
        result->push_back(p->k_);
        p = p->Next(0);
    }
}
