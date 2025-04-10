//
// Created by Lddyss on 2025/2/1.
//
#include <gtest/gtest.h>
#include "skip_list.h"

class SkipListTest : public ::testing::Test {
protected:
    void SetUp() override {
        sk = new SkipList<int, std::less<>>(12, 4);
    }
    void TearDown() override {
        delete sk;
    }
    void RangeTestInit() {
        sk->Insert(10);
        sk->Insert(20);
        sk->Insert(30);
        sk->Insert(40);
        sk->Insert(50);
    }
    OvOSet<int, std::less<>>* sk;
};

// 基础功能测试
TEST_F(SkipListTest, InsertOperations) {
    // 测试空跳表
    EXPECT_FALSE(sk->Contains(0));
    EXPECT_FALSE(sk->Remove(0));

    // 插入并验证
    EXPECT_TRUE(sk->Insert(5));
    EXPECT_TRUE(sk->Contains(5));

    // 重复插入失败
    EXPECT_FALSE(sk->Insert(5));
//    EXPECT_TRUE(sk->Remove(5));
//    EXPECT_FALSE(sk->Contains(5));
}

// 边界值测试
TEST_F(SkipListTest, BoundaryValues) {
    const int MIN_VAL = std::numeric_limits<int>::min();
    const int MAX_VAL = std::numeric_limits<int>::max();

    EXPECT_TRUE(sk->Insert(MIN_VAL));
    EXPECT_TRUE(sk->Insert(MAX_VAL));

    EXPECT_TRUE(sk->Contains(MIN_VAL));
    EXPECT_TRUE(sk->Contains(MAX_VAL));

//    EXPECT_TRUE(sk->Remove(MIN_VAL));
//    EXPECT_TRUE(sk->Remove(MAX_VAL));
}

// 批量操作测试
TEST_F(SkipListTest, BulkOperations) {
    const int TEST_SIZE = 1000;

    // 批量插入
    for (int i = 0; i < TEST_SIZE; ++i) {
        EXPECT_TRUE(sk->Insert(i));
    }

    // 验证存在性
    for (int i = 0; i < TEST_SIZE; ++i) {
        EXPECT_TRUE(sk->Contains(i));
    }

//    // 批量删除
//    for (int i = 0; i < TEST_SIZE; ++i) {
//        EXPECT_TRUE(sk->Remove(i));
//    }
//
//    // 验证全删除
//    for (int i = 0; i < TEST_SIZE; ++i) {
//        EXPECT_FALSE(sk->Contains(i));
//    }
}

// 随机操作测试
TEST_F(SkipListTest, RandomOperations) {
    std::set<int> ref_set;  // 用std::set作为参考

    // 随机插入/删除验证一致性
    Random dis;

    for (int i = 0; i < 5000; ++i) {
        int val = dis(1, 10000);
        if (dis(1, 10) <= 6) {  // 插入操作
            auto expected = ref_set.insert(val).second;
            EXPECT_EQ(expected, sk->Insert(val));
        } else {  // 删除操作
//            auto expected = ref_set.erase(val) > 0;
//            EXPECT_EQ(expected, sk->Remove(val));
        }
    }

    // 最终一致性验证
    for (auto val : ref_set) {
        EXPECT_TRUE(sk->Contains(val));
    }
}

// 异常场景测试
TEST_F(SkipListTest, InvalidOperations) {
    // 插入非法值（假设无限制）
    EXPECT_TRUE(sk->Insert(0));  // 0通常是合法值

    // 验证空跳表删除
    SkipList<int, std::less<>> empty_sk(12, 4);
    EXPECT_FALSE(empty_sk.Remove(0));
}

// 测试正常范围查询
TEST_F(SkipListTest, HandlesNormalRange) {
    RangeTestInit();
    std::vector<int> result_;
    sk->GetRange(15, 45, &result_);
    ASSERT_EQ(3, result_.size());
    EXPECT_EQ(20, result_[0]);
    EXPECT_EQ(30, result_[1]);
    EXPECT_EQ(40, result_[2]);
}

// 测试精确边界匹配
TEST_F(SkipListTest, HandlesExactBoundaries) {
    RangeTestInit();
    std::vector<int> result_;
    sk->GetRange(10, 51, &result_);
    ASSERT_EQ(5, result_.size());
    EXPECT_EQ(10, result_.front());
    EXPECT_EQ(50, result_.back());
}

// 测试空结果集
TEST_F(SkipListTest, HandlesNoResults) {
    RangeTestInit();
    std::vector<int> result_;
    sk->GetRange(60, 100, &result_);
    EXPECT_TRUE(result_.empty());
}

// 测试单元素结果
TEST_F(SkipListTest, HandlesSingleElement) {
    RangeTestInit();
    std::vector<int> result_;
    sk->GetRange(25, 35, &result_);
    ASSERT_EQ(1, result_.size());
    EXPECT_EQ(30, result_[0]);
}

// 测试无效范围（lower > upper）
TEST_F(SkipListTest, HandlesInvalidRange) {
    RangeTestInit();
    std::vector<int> result_;
    sk->GetRange(50, 10, &result_);
    EXPECT_TRUE(result_.empty()); // 或根据设计返回反向结果
}

// 测试空集合
TEST_F(SkipListTest, HandlesEmptySet) {
    RangeTestInit();
    std::vector<int> result_;
    SkipList<int,std::less<>> empty_set;
    std::vector<int> empty_result;
    empty_set.GetRange(0, 100, &empty_result);
    EXPECT_TRUE(empty_result.empty());
}

// 测试完全包含的情况
TEST_F(SkipListTest, HandlesFullContainment) {
    RangeTestInit();
    std::vector<int> result_;
    sk->Insert(5);
    sk->Insert(55);
    sk->GetRange(10, 51, &result_);
    ASSERT_EQ(5, result_.size()); // 应排除新增的 5 和 55
}
