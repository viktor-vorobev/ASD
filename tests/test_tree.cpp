#include <gtest/gtest.h>
#include "tree.h"
#include <string>
#include <stdexcept>

class BinaryTreeTest : public ::testing::Test {
protected:
    BinaryTree<int> int_tree;
    BinaryTree<std::string> str_tree;

    void SetUp() override {
        // Пустые деревья
    }
};

// Тест создания и базовых свойств
TEST_F(BinaryTreeTest, CreateEmptyTree) {
    EXPECT_TRUE(int_tree.empty());
    EXPECT_EQ(int_tree.size(), 0);
    EXPECT_TRUE(str_tree.empty());
    EXPECT_EQ(str_tree.size(), 0);
}

TEST_F(BinaryTreeTest, InsertSingleElement) {
    int_tree.insert(10);
    EXPECT_FALSE(int_tree.empty());
    EXPECT_EQ(int_tree.size(), 1);
    EXPECT_TRUE(int_tree.find(10));
    EXPECT_FALSE(int_tree.find(5));
}

// Тест вставки нескольких элементов
TEST_F(BinaryTreeTest, InsertMultipleElements) {
    int_tree.insert(5);
    int_tree.insert(1);
    int_tree.insert(3);
    int_tree.insert(7);
    int_tree.insert(2);
    int_tree.insert(4);

    EXPECT_EQ(int_tree.size(), 6);
    EXPECT_TRUE(int_tree.find(5));
    EXPECT_TRUE(int_tree.find(1));
    EXPECT_TRUE(int_tree.find(3));
    EXPECT_TRUE(int_tree.find(7));
    EXPECT_TRUE(int_tree.find(2));
    EXPECT_TRUE(int_tree.find(4));
    EXPECT_FALSE(int_tree.find(10));
}

// Тест удаления элемента 
TEST_F(BinaryTreeTest, EraseElement) {
    int_tree.insert(5);
    int_tree.insert(1);
    int_tree.insert(3);
    int_tree.insert(7);
    int_tree.insert(2);
    int_tree.insert(4);

    // Проверяем структуру до удаления
    EXPECT_TRUE(int_tree.find(7));

    // Удаляем элемент 7
    int_tree.erase(7);

    EXPECT_FALSE(int_tree.find(7));
    EXPECT_EQ(int_tree.size(), 5);

    // Удаляем корень 5
    int_tree.erase(5);

    EXPECT_FALSE(int_tree.find(5));
    EXPECT_EQ(int_tree.size(), 4);

    // Проверяем, что остальные элементы на месте
    EXPECT_TRUE(int_tree.find(1));
    EXPECT_TRUE(int_tree.find(2));
    EXPECT_TRUE(int_tree.find(3));
    EXPECT_TRUE(int_tree.find(4));
}

// Тест удаления последнего элемента
TEST_F(BinaryTreeTest, EraseLastElement) {
    int_tree.insert(10);
    int_tree.erase(10);
    EXPECT_TRUE(int_tree.empty());
    EXPECT_EQ(int_tree.size(), 0);
}

// Тест удаления из пустого дерева
TEST_F(BinaryTreeTest, EraseFromEmptyTree) {
    EXPECT_THROW(int_tree.erase(5), std::runtime_error);
}

// Тест удаления несуществующего элемента
TEST_F(BinaryTreeTest, EraseNonExistent) {
    int_tree.insert(5);
    int_tree.insert(3);
    EXPECT_THROW(int_tree.erase(10), std::runtime_error);
}

// Тест поиска элементов
TEST_F(BinaryTreeTest, FindElements) {
    str_tree.insert("apple");
    str_tree.insert("banana");
    str_tree.insert("cherry");

    EXPECT_TRUE(str_tree.find("apple"));
    EXPECT_TRUE(str_tree.find("banana"));
    EXPECT_TRUE(str_tree.find("cherry"));
    EXPECT_FALSE(str_tree.find("orange"));
}

// Тест findPtr
TEST_F(BinaryTreeTest, FindPtr) {
    int_tree.insert(10);
    int_tree.insert(20);

    int* ptr = int_tree.findPtr(10);
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(*ptr, 10);

    *ptr = 100;
    EXPECT_TRUE(int_tree.find(100));
    EXPECT_FALSE(int_tree.find(10));
}

// Тест findPtr с предикатом
TEST_F(BinaryTreeTest, FindPtrWithPredicate) {
    str_tree.insert("hello");
    str_tree.insert("world");
    str_tree.insert("test");

    std::string* ptr = str_tree.findPtr([](const std::string& s) { return s.length() == 4; });
    ASSERT_NE(ptr, nullptr);
    EXPECT_EQ(*ptr, "test");
}

// Тест обходов PreOrder
TEST_F(BinaryTreeTest, TraversePreOrder) {
    int_tree.insert(5);
    int_tree.insert(1);
    int_tree.insert(3);
    int_tree.insert(7);
    int_tree.insert(2);
    int_tree.insert(4);

    std::string result = int_tree.traversePreOrder();
    // PreOrder: корень, левое, правое
    // Ожидаемая структура: 
    //       5
    //     /   \
    //    1     3
    //   / \   /
    //  7   2 4
    std::string expected = "5 1 7 2 3 4";
    EXPECT_EQ(result, expected);
}

// Тест обходов InOrder
TEST_F(BinaryTreeTest, TraverseInOrder) {
    int_tree.insert(5);
    int_tree.insert(1);
    int_tree.insert(3);
    int_tree.insert(7);
    int_tree.insert(2);
    int_tree.insert(4);

    std::string result = int_tree.traverseInOrder();
    // InOrder: левое, корень, правое
    std::string expected = "7 1 2 5 4 3";
    EXPECT_EQ(result, expected);
}

// Тест обходов PostOrder
TEST_F(BinaryTreeTest, TraversePostOrder) {
    int_tree.insert(5);
    int_tree.insert(1);
    int_tree.insert(3);
    int_tree.insert(7);
    int_tree.insert(2);
    int_tree.insert(4);

    std::string result = int_tree.traversePostOrder();
    // PostOrder: левое, правое, корень
    std::string expected = "7 2 1 4 3 5";
    EXPECT_EQ(result, expected);
}

// Тест обходов LevelOrder
TEST_F(BinaryTreeTest, TraverseLevelOrder) {
    int_tree.insert(5);
    int_tree.insert(1);
    int_tree.insert(3);
    int_tree.insert(7);
    int_tree.insert(2);
    int_tree.insert(4);

    std::string result = int_tree.traverseLevelOrder();
    // LevelOrder: по уровням
    std::string expected = "5 1 3 7 2 4";
    EXPECT_EQ(result, expected);
}

// Тест итератора (InOrder обход)
TEST_F(BinaryTreeTest, IteratorTest) {
    int_tree.insert(5);
    int_tree.insert(3);
    int_tree.insert(7);
    int_tree.insert(1);
    int_tree.insert(4);
    int_tree.insert(6);
    int_tree.insert(8);

    std::vector<int> values;
    for (auto it = int_tree.begin(); it != int_tree.end(); ++it) {
        values.push_back(*it);
    }

    // InOrder должен дать отсортированный порядок
    std::vector<int> expected = { 1, 3, 4, 5, 6, 7, 8 };
    EXPECT_EQ(values, expected);
}

// Тест копирования
TEST_F(BinaryTreeTest, CopyConstructor) {
    int_tree.insert(5);
    int_tree.insert(3);
    int_tree.insert(7);

    BinaryTree<int> copy(int_tree);
    EXPECT_EQ(copy.size(), 3);
    EXPECT_TRUE(copy.find(5));
    EXPECT_TRUE(copy.find(3));
    EXPECT_TRUE(copy.find(7));
    EXPECT_EQ(int_tree.traverseLevelOrder(), copy.traverseLevelOrder());
}

// Тест присваивания
TEST_F(BinaryTreeTest, AssignmentOperator) {
    int_tree.insert(5);
    int_tree.insert(3);

    BinaryTree<int> other;
    other.insert(10);
    other.insert(20);
    other.insert(30);

    other = int_tree;
    EXPECT_EQ(other.size(), 2);
    EXPECT_TRUE(other.find(5));
    EXPECT_TRUE(other.find(3));
    EXPECT_FALSE(other.find(10));
}

// Тест перемещения
TEST_F(BinaryTreeTest, MoveConstructor) {
    int_tree.insert(5);
    int_tree.insert(3);
    int_tree.insert(7);

    BinaryTree<int> moved(std::move(int_tree));
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(moved.find(5));
    EXPECT_TRUE(moved.find(3));
    EXPECT_TRUE(moved.find(7));
    EXPECT_TRUE(int_tree.empty()); // Исходное дерево должно быть пустым
}

// Тест getAllElements
TEST_F(BinaryTreeTest, GetAllElements) {
    int_tree.insert(5);
    int_tree.insert(3);
    int_tree.insert(7);

    std::vector<int> elements = int_tree.getAllElements();
    EXPECT_EQ(elements.size(), 3);
    EXPECT_EQ(elements[0], 5);
    EXPECT_EQ(elements[1], 3);
    EXPECT_EQ(elements[2], 7);
}

// Тест clear
TEST_F(BinaryTreeTest, Clear) {
    int_tree.insert(5);
    int_tree.insert(3);
    int_tree.clear();
    EXPECT_TRUE(int_tree.empty());
    EXPECT_EQ(int_tree.size(), 0);
}

// Тест printTree (визуальная проверка)
TEST_F(BinaryTreeTest, PrintTree) {
    int_tree.insert(5);
    int_tree.insert(1);
    int_tree.insert(3);
    int_tree.insert(7);
    int_tree.insert(2);
    int_tree.insert(4);

    // Просто вызываем, чтобы убедиться, что не падает
    testing::internal::CaptureStdout();
    int_tree.printTree();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
}

// Тест структуры после удаления по заданию
TEST_F(BinaryTreeTest, EraseStructureTest) {
    // Воспроизводим пример из задания
    int_tree.insert(5);
    int_tree.insert(1);
    int_tree.insert(3);
    int_tree.insert(7);
    int_tree.insert(2);
    int_tree.insert(4);

    // Исходная структура:
    //       5
    //     /   \
    //    1     3
    //   / \   /
    //  7   2 4

    // Удаляем 7
    int_tree.erase(7);
    // Ожидаемая структура:
    //       5
    //     /   \
    //    1     3
    //   / \   /
    //  4   2
    EXPECT_FALSE(int_tree.find(7));
    EXPECT_TRUE(int_tree.find(5));
    EXPECT_TRUE(int_tree.find(1));
    EXPECT_TRUE(int_tree.find(3));
    EXPECT_TRUE(int_tree.find(2));
    EXPECT_TRUE(int_tree.find(4));

    // Удаляем 5
    int_tree.erase(5);
    // Ожидаемая структура:
    //       2
    //     /   \
    //    1     3
    //   /
    //  4
    EXPECT_FALSE(int_tree.find(5));
    EXPECT_TRUE(int_tree.find(2));
    EXPECT_TRUE(int_tree.find(1));
    EXPECT_TRUE(int_tree.find(3));
    EXPECT_TRUE(int_tree.find(4));
}