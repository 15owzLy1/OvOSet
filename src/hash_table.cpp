//
// Created by Lddyss on 2025/1/30.
//
#include "hash_table.h"

size_t HashTable::hashFunction(int key) const {
    return std::hash<int>()(key) % size;
}

void HashTable::Insert(int key) {
    size_t index = hashFunction(key);
    table[index].keys.push_back(key);
}

bool HashTable::Remove(int key) {
    size_t index = hashFunction(key);
    auto& bucket = table[index];
    auto it = std::find(bucket.keys.begin(), bucket.keys.end(), key);
    if (it != bucket.keys.end()) {
        bucket.keys.erase(it);
        return true;
    }
    return false;
}

bool HashTable::Find(int key) const {
    size_t index = hashFunction(key);
    const auto& bucket = table[index];
    return std::find(bucket.keys.begin(), bucket.keys.end(), key) != bucket.keys.end();
}
