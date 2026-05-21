#include <gtest/gtest.h>
#include "binary_tree.h"

TEST(BinaryTreeTest, InsertAndFind) {
    BinaryTree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    EXPECT_EQ(tree.size(), 3);
    EXPECT_NE(tree.find([](const int& v) { return v == 2; }), nullptr);
    EXPECT_EQ(tree.find([](const int& v) { return v == 4; }), nullptr);
}

TEST(BinaryTreeTest, RemoveNode) {
    BinaryTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    bool removed = tree.remove([](const int& v) { return v == 20; });
    EXPECT_TRUE(removed);
    EXPECT_EQ(tree.size(), 2);
    EXPECT_EQ(tree.find([](const int& v) { return v == 20; }), nullptr);
}