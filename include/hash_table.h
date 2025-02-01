//
// Created by Lddyss on 2025/1/30.
//

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "ovo_set.h"
#include <list>
#include <cstring>
#include <vector>

template <typename Key, class Comparator>
class HashTable : public OvOSet<Key, Comparator> {
private:
    struct hashNode {
        Key k_;
        size_t hash_;
        hashNode* next_;
    };
    struct hashBucket {
        hashNode* head;
    };

    size_t ele_num_, bucket_num_;
    Comparator compare_;
    hashBucket* bucket_;

    bool equal(const Key &i, const Key &j) const {
        return !compare_(i, j) && !compare_(j, i);
    }
    size_t getHash(const Key &key) const;
    hashNode** findPointer(const Key &key, size_t hash) const;
    void resize();

public:
    explicit HashTable(size_t bucket_num = 128);
    bool Insert(const Key &key) override;
    bool Remove(const Key &key) override;
    bool Contains(const Key &key) const override;
};

template class HashTable<int, std::less<>>;

#endif // HASH_TABLE_H