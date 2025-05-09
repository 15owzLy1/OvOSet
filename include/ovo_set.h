//
// Created by Lddyss on 2025/1/30.
//

// include/ovo_set.h
#ifndef OVO_SET_H
#define OVO_SET_H
#include <cstdio>
#include <cstdint>
#include <functional>
#include "random.h"
using std::size_t;

template <typename Key, class Comparator>
class OvOSet {
public:
    OvOSet() : memory_usage_(0) {}
    virtual ~OvOSet() = default;
    virtual bool Insert(const Key &key) = 0;
    virtual bool Remove(const Key &key) = 0;
    virtual bool Contains(const Key &key) = 0;
    virtual void GetRange(const Key &lower, const Key &upper, std::vector<Key>* result) = 0;
    uint64_t memory_usage_;
};

#endif // OVO_SET_H
