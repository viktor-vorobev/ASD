#include <gtest/gtest.h>
#include "rb_tree.h"
#include <string>

TEST(RBTreeTest, InsertAndFind) {
    RBTree<int, std::string> tree;
    EXPECT_TRUE(tree.insert(10, "A"));
    EXPECT_TRUE(tree.insert(20, "B"));
    EXPECT_TRUE(tree.insert(5, "C"));

    EXPECT_EQ(tree.size(), 3);
    auto* val = tree.find(20);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, "B");
}