#include <gtest/gtest.h>
#include "bst.h"
#include <string>

TEST(BSTTest, InsertAndFind) {
    BST<int, std::string> tree;
    EXPECT_TRUE(tree.insert(10, "A"));
    EXPECT_TRUE(tree.insert(5, "B"));
    EXPECT_FALSE(tree.insert(10, "C")); // Дубликат

    auto* val = tree.find(5);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, "B");
}

TEST(BSTTest, EraseNode) {
    BST<int, std::string> tree;
    tree.insert(10, "A");
    tree.insert(5, "B");
    tree.insert(15, "C");

    EXPECT_TRUE(tree.erase(5));
    EXPECT_EQ(tree.size(), 2);
    EXPECT_EQ(tree.find(5), nullptr);
}