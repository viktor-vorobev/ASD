#include <gtest/gtest.h>
#include "list.h"
#include <string>

// Тесты для конструкторов
TEST(ListTest, DefaultConstructor) {
    List<int> list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(ListTest, CopyConstructor) {
    List<int> original;
    original.push_back(1);
    original.push_back(2);
    original.push_back(3);

    List<int> copy(original);
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.front(), 1);
    EXPECT_EQ(copy.back(), 3);
}

TEST(ListTest, MoveConstructor) {
    List<int> original;
    original.push_back(1);
    original.push_back(2);

    List<int> moved(std::move(original));
    EXPECT_EQ(moved.size(), 2);
    EXPECT_TRUE(original.empty());
}

// Тесты для операторов присваивания
TEST(ListTest, CopyAssignment) {
    List<int> list1;
    list1.push_back(1);
    list1.push_back(2);

    List<int> list2;
    list2 = list1;

    EXPECT_EQ(list2.size(), 2);
    EXPECT_EQ(list2.front(), 1);
    EXPECT_EQ(list2.back(), 2);
}

TEST(ListTest, MoveAssignment) {
    List<int> list1;
    list1.push_back(1);
    list1.push_back(2);

    List<int> list2;
    list2 = std::move(list1);

    EXPECT_EQ(list2.size(), 2);
    EXPECT_TRUE(list1.empty());
}

// Тесты для вставки элементов
TEST(ListTest, PushFront) {
    List<int> list;
    list.push_front(1);
    list.push_front(2);

    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(list.size(), 2);
}

TEST(ListTest, PushBack) {
    List<int> list;
    list.push_back(1);
    list.push_back(2);

    EXPECT_EQ(list.back(), 2);
    EXPECT_EQ(list.size(), 2);
}

TEST(ListTest, Insert) {
    List<int> list;
    list.push_back(1);
    list.push_back(3);
    list.insert(1, 2);

    EXPECT_EQ(list.size(), 3);

    auto it = list.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
}

// Тесты для удаления элементов
TEST(ListTest, PopFront) {
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.pop_front();

    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(list.size(), 1);
}

TEST(ListTest, PopBack) {
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.pop_back();

    EXPECT_EQ(list.back(), 1);
    EXPECT_EQ(list.size(), 1);
}

TEST(ListTest, Erase) {
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.erase(1);

    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(ListTest, Clear) {
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.clear();

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

// Тесты для доступа к элементам
TEST(ListTest, FrontAndBack) {
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(ListTest, FrontThrowsWhenEmpty) {
    List<int> list;
    EXPECT_THROW(list.front(), std::runtime_error);
}

TEST(ListTest, BackThrowsWhenEmpty) {
    List<int> list;
    EXPECT_THROW(list.back(), std::runtime_error);
}

// Тесты для поиска
TEST(ListTest, Contains) {
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    EXPECT_TRUE(list.contains(2));
    EXPECT_FALSE(list.contains(4));
}

TEST(ListTest, Find) {
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    EXPECT_EQ(list.find(2), 1);
    EXPECT_EQ(list.find(4), -1);
}

// Тесты для итераторов
TEST(ListTest, Iterator) {
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    int sum = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        sum += *it;
    }

    EXPECT_EQ(sum, 6);
}

TEST(ListTest, RangeBasedFor) {
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    int sum = 0;
    for (int value : list) {
        sum += value;
    }

    EXPECT_EQ(sum, 6);
}

// Тесты с разными типами данных
TEST(ListTest, StringList) {
    List<std::string> list;
    list.push_back("hello");
    list.push_back("world");

    EXPECT_EQ(list.front(), "hello");
    EXPECT_EQ(list.back(), "world");
    EXPECT_TRUE(list.contains("hello"));
}

TEST(ListTest, DoubleList) {
    List<double> list;
    list.push_back(1.1);
    list.push_back(2.2);
    list.push_back(3.3);

    EXPECT_DOUBLE_EQ(list.front(), 1.1);
    EXPECT_DOUBLE_EQ(list.back(), 3.3);
}

// Тесты на граничные случаи
TEST(ListTest, InsertAtBoundaries) {
    List<int> list;
    list.insert(0, 1); // Вставка в пустой список
    list.insert(0, 0); // Вставка в начало
    list.insert(2, 2); // Вставка в конец

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 0);
    EXPECT_EQ(list.back(), 2);
}

TEST(ListTest, EraseFromSingleElement) {
    List<int> list;
    list.push_back(1);
    list.erase(0);

    EXPECT_TRUE(list.empty());
}

// Тесты на исключения
TEST(ListTest, InsertOutOfRange) {
    List<int> list;
    EXPECT_THROW(list.insert(1, 1), std::out_of_range);
}

TEST(ListTest, EraseOutOfRange) {
    List<int> list;
    list.push_back(1);
    EXPECT_THROW(list.erase(1), std::out_of_range);
}

TEST(ListTest, PopFrontEmpty) {
    List<int> list;
    EXPECT_THROW(list.pop_front(), std::runtime_error);
}

TEST(ListTest, PopBackEmpty) {
    List<int> list;
    EXPECT_THROW(list.pop_back(), std::runtime_error);
}

