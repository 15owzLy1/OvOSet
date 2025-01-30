//
// Created by Lddyss on 2025/1/30.
//

// ovo_set.h
#ifndef OVO_SET_H
#define OVO_SET_H

#include <iostream>
#include <functional>

class OvOSet {
public:
    virtual ~OvOSet() {}
    virtual void Insert(int key) = 0;
    virtual bool Remove(int key) = 0;
    virtual bool Find(int key) const = 0;
};

#endif // OVO_SET_H
