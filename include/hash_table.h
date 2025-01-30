//
// Created by Lddyss on 2025/1/30.
//

#ifndef CODE_HASH_TABLE_H
#define CODE_HASH_TABLE_H

#include "ovo_set.h"
#include <iostream>
#include <vector>
#include <list>

class HashTable : public OvOSet {
private:
    struct Bucket {
        std::list<int> keys;
    };

    std::vector<Bucket> table;
    size_t size;

    size_t hashFunction(int key) const;

public:
    HashTable(size_t initialSize = 101) : size(initialSize), table(size) {}
    void Insert(int key) override;
    bool Remove(int key) override;
    bool Find(int key) const override;
};

#endif //CODE_HASH_TABLE_H
