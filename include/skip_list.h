//
// Created by Lddyss on 2025/1/30.
//

#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cassert>
#include "ovo_set.h"

template <typename Key, class Comparator>
class SkipList : public OvOSet<Key, Comparator> {
private:
    struct SkipListNode;

    const uint16_t maxLevel_;
    const uint16_t branchNum_;
    Comparator compare_;
    std::atomic<int> cur_level_;
    SkipListNode* const head_;
    Random rand_;

    int getRandomLevel();
    inline int getCurrentLevel() const {
        return cur_level_.load(std::memory_order_acquire);
    }
    bool equal(const Key &i, const Key &j) const {
        return !compare_(i, j) && !compare_(j, i);
    }
    SkipListNode* lowerBound(const Key &k, SkipListNode** prev) const;

public:
    explicit SkipList(uint16_t max_level = 12, uint16_t branch_num = 4);
    SkipList(const SkipList&) = delete;
    SkipList& operator=(const SkipList&) = delete;

    ~SkipList();

    bool Insert(const Key& key) override;

    bool Contains(const Key& key) override;

    bool Remove(const Key& key) override;

    void GetRange(const Key &lower, const Key &upper, std::vector<Key>* result) override;

    void Print() const;
};

template class SkipList<int, std::less<>>;
template class SkipList<uint32_t, std::less<>>;
template class SkipList<uint64_t, std::less<>>;
template class SkipList<std::string, std::less<>>;

#endif // SKIP_LIST_H
