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
    void RangeTestInit() {
        ht->Insert(10);
        ht->Insert(20);
        ht->Insert(30);
        ht->Insert(40);
        ht->Insert(50);
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

// 测试正常范围查询
TEST_F(HashTableTest, HandlesNormalRange) {
    RangeTestInit();
    std::vector<int> result_;
    ht->GetRange(15, 45, &result_);
    ASSERT_EQ(3, result_.size());
    EXPECT_EQ(20, result_[0]);
    EXPECT_EQ(30, result_[1]);
    EXPECT_EQ(40, result_[2]);
}

// 测试精确边界匹配
TEST_F(HashTableTest, HandlesExactBoundaries) {
    RangeTestInit();
    std::vector<int> result_;
    ht->GetRange(10, 50, &result_);
    ASSERT_EQ(5, result_.size());
    EXPECT_EQ(10, result_.front());
    EXPECT_EQ(50, result_.back());
}

// 测试空结果集
TEST_F(HashTableTest, HandlesNoResults) {
    RangeTestInit();
    std::vector<int> result_;
    ht->GetRange(60, 100, &result_);
    EXPECT_TRUE(result_.empty());
}

// 测试单元素结果
TEST_F(HashTableTest, HandlesSingleElement) {
    RangeTestInit();
    std::vector<int> result_;
    ht->GetRange(25, 35, &result_);
    ASSERT_EQ(1, result_.size());
    EXPECT_EQ(30, result_[0]);
}

// 测试无效范围（lower > upper）
TEST_F(HashTableTest, HandlesInvalidRange) {
    RangeTestInit();
    std::vector<int> result_;
    ht->GetRange(50, 10, &result_);
    EXPECT_TRUE(result_.empty()); // 或根据设计返回反向结果
}

// 测试空集合
TEST_F(HashTableTest, HandlesEmptySet) {
    RangeTestInit();
    std::vector<int> result_;
    HashTable<int, std::less<>> empty_set;
    std::vector<int> empty_result;
    empty_set.GetRange(0, 100, &empty_result);
    EXPECT_TRUE(empty_result.empty());
}

// 测试完全包含的情况
TEST_F(HashTableTest, HandlesFullContainment) {
    RangeTestInit();
    std::vector<int> result_;
    ht->Insert(5);
    ht->Insert(55);
    ht->GetRange(10, 50, &result_);
    ASSERT_EQ(5, result_.size()); // 应排除新增的 5 和 55
}
