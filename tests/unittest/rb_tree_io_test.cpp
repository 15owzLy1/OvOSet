//
// Created by Lddyss on 2025/2/14.
//
#include "rb_tree.h"
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

class RBTreeTest : public ::testing::Test {
protected:
    void SetUp() override {
        rb = new RBTree<int, std::less<>>;
    }
    void TearDown() override {
        delete rb;
    }
    void RangeTestInit() {
        rb->Insert(10);
        rb->Insert(20);
        rb->Insert(30);
        rb->Insert(40);
        rb->Insert(50);
    }
    OvOSet<int, std::less<>>* rb;
};

// 测试 Insert() 接口
TEST_F(RBTreeTest, InsertTest) {
    // 插入少量元素
    std::vector<int> small_set = {5, 10, 15};
    for (int val : small_set) {
        rb->Insert(val);
    }
    for (int val : small_set) {
        EXPECT_TRUE(rb->Contains(val)) << "Element " << val << " should be in the tree.";
    }

    // 插入重复元素
    rb->Insert(5);
    rb->Insert(10);
    EXPECT_TRUE(rb->Contains(5));
    EXPECT_TRUE(rb->Contains(10));
}

// 测试 Contains() 接口
TEST_F(RBTreeTest, ContainsTest) {
    // 插入一些元素
    std::vector<int> elements = {1, 2, 3, 5, 7, 11, 13};
    for (int val : elements) {
        rb->Insert(val);
    }

    // 验证已插入的元素存在
    for (int val : elements) {
        EXPECT_TRUE(rb->Contains(val)) << "Element " << val << " should be in the tree.";
    }

    // 验证未插入的元素不存在
    std::vector<int> non_elements = {4, 6, 8, 9, 10, 12};
    for (int val : non_elements) {
        EXPECT_FALSE(rb->Contains(val)) << "Element " << val << " should not be in the tree.";
    }
}

// 测试 Remove() 接口
TEST_F(RBTreeTest, RemoveTest) {
    // 插入一些元素
    std::vector<int> elements = {1, 2, 3, 5, 7, 11, 13};
    for (int val : elements) {
        rb->Insert(val);
    }

    // 删除一个元素
    rb->Remove(5);
    EXPECT_FALSE(rb->Contains(5)) << "Element 5 should be removed.";
    for (int val : elements) {
        if (val != 5) {
            EXPECT_TRUE(rb->Contains(val)) << "Element " << val << " should still be in the tree.";
        }
    }

    // 删除不存在的元素
    rb->Remove(100);
    EXPECT_FALSE(rb->Contains(100)) << "Element 100 should not be in the tree.";

    // 删除所有元素
    for (int val : elements) {
        if (val != 5) {  // 5 已经被删除
            rb->Remove(val);
            EXPECT_FALSE(rb->Contains(val)) << "Element " << val << " should be removed.";
        }
    }
}

// 测试大量数据的插入和删除
TEST_F(RBTreeTest, LargeDataTest) {
    // 插入大量数据
    std::vector<int> large_set(10000);
    std::iota(large_set.begin(), large_set.end(), 0);  // 生成 0 到 9999 的整数序列
    for (int val : large_set) {
        rb->Insert(val);
    }

    // 验证所有元素都存在
    for (int val : large_set) {
        EXPECT_TRUE(rb->Contains(val)) << "Element " << val << " should be in the tree.";
    }

    // 删除一半的数据
    for (size_t i = 0; i < large_set.size() / 2; ++i) {
        rb->Remove(large_set[i]);
        EXPECT_FALSE(rb->Contains(large_set[i])) << "Element " << large_set[i] << " should be removed.";
    }

    // 验证剩余的元素仍然存在
    for (size_t i = large_set.size() / 2; i < large_set.size(); ++i) {
        EXPECT_TRUE(rb->Contains(large_set[i])) << "Element " << large_set[i] << " should still be in the tree.";
    }
}

// 测试边界条件
TEST_F(RBTreeTest, BoundaryTest) {
    // 插入边界值
    rb->Insert(std::numeric_limits<int>::min());
    rb->Insert(std::numeric_limits<int>::max());

    EXPECT_TRUE(rb->Contains(std::numeric_limits<int>::min()));
    EXPECT_TRUE(rb->Contains(std::numeric_limits<int>::max()));

    // 删除边界值
    rb->Remove(std::numeric_limits<int>::min());
    rb->Remove(std::numeric_limits<int>::max());

    EXPECT_FALSE(rb->Contains(std::numeric_limits<int>::min()));
    EXPECT_FALSE(rb->Contains(std::numeric_limits<int>::max()));
}

// 测试正常范围查询
TEST_F(RBTreeTest, HandlesNormalRange) {
    RangeTestInit();
    std::vector<int> result_;
    rb->GetRange(15, 45, &result_);
    ASSERT_EQ(3, result_.size());
    EXPECT_EQ(20, result_[0]);
    EXPECT_EQ(30, result_[1]);
    EXPECT_EQ(40, result_[2]);
}

// 测试精确边界匹配
TEST_F(RBTreeTest, HandlesExactBoundaries) {
    RangeTestInit();
    std::vector<int> result_;
    rb->GetRange(10, 51, &result_);
    ASSERT_EQ(5, result_.size());
    EXPECT_EQ(10, result_.front());
    EXPECT_EQ(50, result_.back());
}

// 测试空结果集
TEST_F(RBTreeTest, HandlesNoResults) {
    RangeTestInit();
    std::vector<int> result_;
    rb->GetRange(60, 100, &result_);
    EXPECT_TRUE(result_.empty());
}

// 测试单元素结果
TEST_F(RBTreeTest, HandlesSingleElement) {
    RangeTestInit();
    std::vector<int> result_;
    rb->GetRange(25, 35, &result_);
    ASSERT_EQ(1, result_.size());
    EXPECT_EQ(30, result_[0]);
}

// 测试无效范围（lower > upper）
TEST_F(RBTreeTest, HandlesInvalidRange) {
    RangeTestInit();
    std::vector<int> result_;
    rb->GetRange(50, 10, &result_);
    EXPECT_TRUE(result_.empty()); // 或根据设计返回反向结果
}

// 测试空集合
TEST_F(RBTreeTest, HandlesEmptySet) {
    RangeTestInit();
    std::vector<int> result_;
    RBTree<int, std::less<>> empty_set;
    std::vector<int> empty_result;
    empty_set.GetRange(0, 100, &empty_result);
    EXPECT_TRUE(empty_result.empty());
}

// 测试完全包含的情况
TEST_F(RBTreeTest, HandlesFullContainment) {
    RangeTestInit();
    std::vector<int> result_;
    rb->Insert(5);
    rb->Insert(55);
    rb->GetRange(10, 51, &result_);
    ASSERT_EQ(5, result_.size()); // 应排除新增的 5 和 55
}