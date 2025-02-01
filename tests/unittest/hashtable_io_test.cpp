//
// Created by Lddyss on 2025/1/31.
//
#include "gtest/gtest.h"
#include "hash_table.h"

class HashTableTest : public ::testing::Test {
protected:
    void SetUp() override {
        ht = new HashTable<int, std::less<>>();
    }
    void TearDown() override {
        delete ht;
    }
    OvOSet<int, std::less<>>* ht;
};

// 测试插入和查找基础功能
TEST_F(HashTableTest, InsertAndFind) {
    EXPECT_TRUE(ht->Insert(5));       // 插入新元素
    EXPECT_TRUE(ht->Contains(5));         // 存在元素应能找到
    EXPECT_FALSE(ht->Insert(5));      // 重复插入应失败
    EXPECT_FALSE(ht->Contains(42));       // 不存在的元素找不到
}

// 测试删除功能
TEST_F(HashTableTest, Remove) {
    ht->Insert(10);
    EXPECT_TRUE(ht->Remove(10));      // 删除存在的元素
    EXPECT_FALSE(ht->Contains(10));       // 确认已删除
    EXPECT_FALSE(ht->Remove(99));     // 删除不存在的元素应失败
}

// 测试边界值（0、负数）
TEST_F(HashTableTest, EdgeCases) {
    EXPECT_TRUE(ht->Insert(0));
    EXPECT_TRUE(ht->Insert(-1));
    EXPECT_TRUE(ht->Contains(0));
    EXPECT_TRUE(ht->Contains(-1));
    EXPECT_TRUE(ht->Remove(-1));
    EXPECT_FALSE(ht->Contains(-1));
}

// 测试哈希表扩容（需根据实现调整阈值）
TEST_F(HashTableTest, ResizeBehavior) {
    // 默认 bucket_num = 128
    for (int i = 0; i < 2050; ++i) {
        ASSERT_TRUE(ht->Insert(i));
    }
    // 验证所有元素存在
    for (int i = 0; i < 2050; ++i) {
        EXPECT_TRUE(ht->Contains(i)) << "i=" << i;
    }
}

// 测试空表操作
TEST_F(HashTableTest, EmptyTable) {
    EXPECT_FALSE(ht->Remove(0));      // 空表删除失败
    EXPECT_FALSE(ht->Contains(0));        // 空表查找失败
}