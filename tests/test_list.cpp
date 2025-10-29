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
TEST(IteratorTest, Iterator) {
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

TEST(IteratorTest, ModifyInLoop) {
    List<int> list;
    for (int i = 1; i <= 5; i++) {
        list.push_back(i);
    }

    // Удвоение всех элементов в цикле
    for (auto it = list.begin(); it != list.end(); ++it) {
        *it = *it * 2;
    }

    // Проверка результатов
    auto it = list.begin();
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 4);
    ++it;
    EXPECT_EQ(*it, 6);
    ++it;
    EXPECT_EQ(*it, 8);
    ++it;
    EXPECT_EQ(*it, 10);
}

// итератор для пустого списка
TEST(IteratorTest, BeginEndEqualityForEmptyList) {
    List<int> empty_list;

    // begin() и end() должны быть равны для пустого списка
    EXPECT_TRUE(empty_list.begin() == empty_list.end());
    EXPECT_FALSE(empty_list.begin() != empty_list.end());

    // Не должно быть падения или неопределенного поведения
    SUCCEED(); // Если дошли сюда - нет падения
}

TEST(IteratorTest, IncrementBeginIteratorOfEmptyList) {
    List<int> empty_list;

    auto it = empty_list.begin();

    // Пре-инкремент не должен падать
    ++it;
    EXPECT_TRUE(it == empty_list.end());

    // Повторный инкремент не должен падать
    ++it;
    EXPECT_TRUE(it == empty_list.end());

    // Многократный инкремент не должен падать
    for (int i = 0; i < 10; ++i) {
        ++it;
    }
    EXPECT_TRUE(it == empty_list.end());
}

TEST(IteratorTest, PostIncrementBeginIteratorOfEmptyList) {
    List<int> empty_list;

    auto it = empty_list.begin();

    // Пост-инкремент не должен падать
    auto old_it = it++;
    EXPECT_TRUE(old_it == empty_list.begin());
    EXPECT_TRUE(it == empty_list.end());

    // Повторный пост-инкремент не должен падать
    old_it = it++;
    EXPECT_TRUE(old_it == empty_list.end());
    EXPECT_TRUE(it == empty_list.end());
}

TEST(IteratorTest, RangeBasedFor) {
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

