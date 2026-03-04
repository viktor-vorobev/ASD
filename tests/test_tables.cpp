#include <gtest/gtest.h>
#include <string>
#include <map>
#include "monom.cpp"
#include "polynom.cpp"
#include "1_table.cpp"
#include "2_table.cpp"
#include "3_table.cpp"

using namespace std;

// Вспомогательная функция для проверки наличия элемента в векторе
template<typename T>
bool vectorContains(const vector<T>& vec, const T& value) {
    return find(vec.begin(), vec.end(), value) != vec.end();
}

// Вспомогательная функция для проверки что векторы содержат одни и те же элементы
template<typename T>
void expectUnorderedEquality(const vector<T>& actual, const vector<T>& expected) {
    ASSERT_EQ(actual.size(), expected.size());
    for (const auto& val : expected) {
        EXPECT_TRUE(vectorContains(actual, val)) << "Value " << val << " not found";
    }
}

// ========== ТЕСТЫ ДЛЯ UnsortedTableOnArr ==========

class UnsortedTableOnArrTest : public ::testing::Test {
protected:
    void SetUp() override {
        tableInt.insert(1, 100);
        tableInt.insert(2, 200);
        tableInt.insert(3, 300);
    }

    UnsortedTableOnArr<int, int> tableInt;
};

TEST_F(UnsortedTableOnArrTest, InsertAndFind) {
    EXPECT_EQ(tableInt.find(1), 100);
    EXPECT_EQ(tableInt.find(2), 200);
    EXPECT_EQ(tableInt.find(3), 300);
    EXPECT_EQ(tableInt.size(), 3);
}

TEST_F(UnsortedTableOnArrTest, Contains) {
    EXPECT_TRUE(tableInt.contains(1));
    EXPECT_TRUE(tableInt.contains(2));
    EXPECT_TRUE(tableInt.contains(3));
    EXPECT_FALSE(tableInt.contains(4));
}

TEST_F(UnsortedTableOnArrTest, GetKeys) {
    auto keys = tableInt.getKeys();
    vector<int> expected = { 1, 2, 3 };
    expectUnorderedEquality(keys, expected);
}

TEST_F(UnsortedTableOnArrTest, GetValues) {
    auto values = tableInt.getValues();
    vector<int> expected = { 100, 200, 300 };
    expectUnorderedEquality(values, expected);
}

// ========== ТЕСТЫ ДЛЯ SortedTableOnArr ==========

class SortedTableOnArrTest : public ::testing::Test {
protected:
    void SetUp() override {
        tableInt.insert(3, 300);
        tableInt.insert(1, 100);
        tableInt.insert(4, 400);
        tableInt.insert(2, 200);
    }

    SortedTableOnArr<int, int> tableInt;
};

TEST_F(SortedTableOnArrTest, KeysAreSorted) {
    auto keys = tableInt.getKeys();
    ASSERT_EQ(keys.size(), 4);
    EXPECT_EQ(keys[0], 1);
    EXPECT_EQ(keys[1], 2);
    EXPECT_EQ(keys[2], 3);
    EXPECT_EQ(keys[3], 4);
}

TEST_F(SortedTableOnArrTest, BinarySearchWorks) {
    EXPECT_EQ(tableInt.find(1), 100);
    EXPECT_EQ(tableInt.find(2), 200);
    EXPECT_EQ(tableInt.find(3), 300);
    EXPECT_EQ(tableInt.find(4), 400);
}

// ========== ТЕСТЫ ДЛЯ UnsortedTableOnList ==========

class UnsortedTableOnListTest : public ::testing::Test {
protected:
    void SetUp() override {
        tableChar.insert('a', 1.5);
        tableChar.insert('b', 2.5);
        tableChar.insert('c', 3.5);
    }

    UnsortedTableOnList<char, double> tableChar;
};

TEST_F(UnsortedTableOnListTest, InsertAndFind) {
    EXPECT_EQ(tableChar.find('a'), 1.5);
    EXPECT_EQ(tableChar.find('b'), 2.5);
    EXPECT_EQ(tableChar.find('c'), 3.5);
    EXPECT_EQ(tableChar.size(), 3);
}

TEST_F(UnsortedTableOnListTest, Contains) {
    EXPECT_TRUE(tableChar.contains('a'));
    EXPECT_TRUE(tableChar.contains('b'));
    EXPECT_TRUE(tableChar.contains('c'));
    EXPECT_FALSE(tableChar.contains('d'));
}

TEST_F(UnsortedTableOnListTest, GetKeys) {
    auto keys = tableChar.getKeys();
    vector<char> expected = { 'a', 'b', 'c' };
    expectUnorderedEquality(keys, expected);
}

// ========== ТЕСТЫ С ПОЛИНОМАМИ ==========

TEST(PolynomTableTests, BasicOperations) {
    UnsortedTableOnArr<string, Polynom> table;

    table.insert("p1", Polynom("x^2+2x+1"));
    table.insert("p2", Polynom("3x^2+4x+5"));

    EXPECT_EQ(table.size(), 2);
    EXPECT_TRUE(table.contains("p1"));
    EXPECT_TRUE(table.contains("p2"));

    map<char, double> values = { {'x', 2} };
    EXPECT_DOUBLE_EQ(table.find("p1").evaluate(values), 9.0);
    EXPECT_DOUBLE_EQ(table.find("p2").evaluate(values), 25.0);
}

// ========== ТЕСТЫ НА ИСКЛЮЧЕНИЯ ==========

TEST(TableExceptionTests, FindNonExistent) {
    UnsortedTableOnArr<int, int> table;
    EXPECT_THROW(table.find(1), TableException);
}

TEST(TableExceptionTests, RemoveNonExistent) {
    SortedTableOnArr<int, int> table;
    EXPECT_THROW(table.remove(1), TableException);
}

// ========== MAIN ==========

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}