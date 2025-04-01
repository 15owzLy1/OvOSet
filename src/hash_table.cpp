//
// Created by Lddyss on 2025/1/30.
//
#include <cassert>
#include <iostream>
#include "hash_table.h"

template <typename Key, class Comparator>
HashTable<Key, Comparator>::HashTable(size_t bucket_num)
    : OvOSet<Key, Comparator>(), ele_num_(0), bucket_num_(bucket_num), mutex_() {
    bucket_ = new hashBucket[bucket_num_];
    memset(bucket_, 0, sizeof(hashBucket) * bucket_num_);
    hash_seed_ = Random64{}();
    this->memory_usage_ += bucket_num_ * sizeof(hashBucket);
}

template <typename Key, class Comparator>
size_t HashTable<Key, Comparator>::getHash(const Key& key) const {
    return XXHash::hash(static_cast<const void*>(&key), sizeof(key), hash_seed_);
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
        auto p = bucket_[i].head;
        while (p != nullptr) {
            auto next = p->next_;
            auto hash = p->hash_;
            auto new_h = &new_bucket[hash & (new_bucket_num - 1)].head;
            p->next_ = *new_h;
            *new_h = p;
            p = next;
            ++cnt;
        }
    }
    assert(cnt == ele_num_);
    delete[] bucket_;
    this->memory_usage_ -= bucket_num_ * sizeof(hashBucket);
    bucket_ = new_bucket;
    bucket_num_ = new_bucket_num;
    this->memory_usage_ += bucket_num_ * sizeof(hashBucket);
}

template <typename Key, class Comparator>
typename HashTable<Key, Comparator>::hashNode**
HashTable<Key, Comparator>::findPointer(const Key &key, const size_t hash) const {
    auto p = &bucket_[hash & (bucket_num_ - 1)].head;
    for (; *p != nullptr && ((*p)->hash_ != hash || !equal(key, (*p)->k_)); p = &(*p)->next_);
    return p;
}

template <typename Key, class Comparator>
bool HashTable<Key, Comparator>::Insert(const Key& key) {
    std::lock_guard l(mutex_);
    auto hash = getHash(key);
    auto ret = findPointer(key, hash);
    auto p = *ret;
    if (p != nullptr) {
        return false;
    }
    auto new_p = new hashNode{key, hash, nullptr};
    *ret = new_p;
    ++ele_num_;
    this->memory_usage_ += sizeof(hashNode);
    if (bucket_num_ < ele_num_) {
        resize();
    }
    return true;
}

template <typename Key, class Comparator>
bool HashTable<Key, Comparator>::Remove(const Key& key) {
    std::lock_guard l(mutex_);
    auto hash = getHash(key);
    auto ret = findPointer(key, hash);
    auto p = *ret;
    if (p == nullptr) {
        return false;
    }
    *ret = p->next_;
    delete p;
    --ele_num_;
    return true;
}

template <typename Key, class Comparator>
bool HashTable<Key, Comparator>::Contains(const Key& key) {
    std::lock_guard l(mutex_);
    return *findPointer(key, getHash(key)) != nullptr;
}

template <typename Key, class Comparator>
void HashTable<Key, Comparator>::GetRange(const Key &lower, const Key &upper, std::vector<Key>* result) {
    if (upper - lower > ele_num_ * 2) {
        std::lock_guard l(mutex_);
        for (size_t i = 0; i < bucket_num_; ++i) {
            auto p = bucket_[i].head;
            while (p != nullptr) {
                if (!compare_(p->k_, lower) && compare_(p->k_, upper)) {
                    result->push_back(p->k_);
                }
                p = p->next_;
            }
        }
    } else {
        for (Key i = lower; i <= upper; ++i) {
            if (Contains(i)) {
                result->push_back(i);
            }
        }
    }
}
