#include <gtest/gtest.h>
#include "avl_tree.h"
#include <string>

TEST(AVLTreeTest, InsertAndBalanceCheck) {
    AVLTree<int, std::string> tree;
    // Вставляем элементы по возрастанию, чтобы спровоцировать повороты
    tree.insert(1, "A");
    tree.insert(2, "B");
    tree.insert(3, "C");

    EXPECT_EQ(tree.size(), 3);
    EXPECT_NE(tree.find(2), nullptr); // 2 должно стать корнем
}

TEST(AVLTreeTest, Erase) {
    AVLTree<int, std::string> tree;
    tree.insert(10, "A");
    tree.insert(20, "B");
    tree.insert(30, "C");

    EXPECT_TRUE(tree.erase(20));
    EXPECT_EQ(tree.size(), 2);
    EXPECT_EQ(tree.find(20), nullptr);
}