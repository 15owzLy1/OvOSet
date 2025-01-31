//
// Created by Lddyss on 2025/1/30.
//
#include <functional>
#include <cassert>
#include "hash_table.h"

template <typename Key, class Comparator>
HashTable<Key, Comparator>::HashTable(size_t bucket_num) : ele_num_(0), bucket_num_(bucket_num) {
    bucket_ = new hashBucket[bucket_num_];
    memset(bucket_, 0, sizeof(hashBucket) * bucket_num_);
}

template <typename Key, class Comparator>
size_t HashTable<Key, Comparator>::getHash(const Key& key) const {
    return std::hash<Key>{}(key);
}

template <typename Key, class Comparator>
typename HashTable<Key, Comparator>::hashNode* HashTable<Key, Comparator>
        ::findPointer(const Key &key, const size_t hash) const {
    auto p = &bucket_[hash & (bucket_num_ - 1)].head;
    for (; p->next_ != nullptr; p = p->next_) {
        if (compare_(p->next_->k_, key) == 0) {
            break;
        }
    }
    return p;
}

template <typename Key, class Comparator>
void HashTable<Key, Comparator>::resize() {
    auto new_bucket_num = bucket_num_;
    while (new_bucket_num < ele_num_) {
        new_bucket_num <<= 1;
    }
    auto new_bucket = new hashBucket[new_bucket_num];
    memset(new_bucket, 0, sizeof(hashBucket) * new_bucket_num);
    // move elements to new buckets
    size_t cnt = 0;
    for (size_t i = 0; i < bucket_num_; ++i) {
        auto p = bucket_[i].head.next_;
        while (p != nullptr) {
            auto next = p->next_;
            auto hash = p->hash_;
            hashNode &new_h = new_bucket[hash & (new_bucket_num - 1)].head;
            p->next_ = new_h.next_;
            new_h.next_ = p;
            p = next;
            ++cnt;
        }
    }
    assert(cnt == ele_num_);
    delete[] bucket_;
    bucket_ = new_bucket;
    bucket_num_ = new_bucket_num;
}

template <typename Key, class Comparator>
bool HashTable<Key, Comparator>::Insert(const Key& key) {
    auto hash = getHash(key);
    auto ret = findPointer(key, hash);
    if (ret->next_ != nullptr) {
        return false;
    }
    auto p = new hashNode{key, hash, ret->next_};
    ret->next_ = p;
    ++ele_num_;
    if (bucket_num_ < ele_num_) {
        resize();
    }
    return true;
}

template <typename Key, class Comparator>
bool HashTable<Key, Comparator>::Remove(const Key& key) {
    auto hash = getHash(key);
    auto ret = findPointer(key, hash);
    if (ret->next_ == nullptr) {
        return false;
    }
    auto p = ret->next_;
    ret->next_ = ret->next_->next_;
    delete []p;
    --ele_num_;
    return true;
}

template <typename Key, class Comparator>
bool HashTable<Key, Comparator>::Find(const Key& key) const {
    auto hash = getHash(key);
    auto ret = findPointer(key, hash);
    return ret->next_ != nullptr;
}
