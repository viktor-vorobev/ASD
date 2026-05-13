#include <gtest/gtest.h>
#include <string>
#include <map>
#include "monom.cpp"
#include "polynom.cpp"
#include "1_table.cpp"
#include "2_table.cpp"
#include "3_table.cpp"
#include "4_table.h"

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

// UnsortedTableOnArr 

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

// SortedTableOnArr

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

//  UnsortedTableOnList 

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

// с полиномами

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

// на исключения

TEST(TableExceptionTests, FindNonExistent) {
    UnsortedTableOnArr<int, int> table;
    EXPECT_THROW(table.find(1), TableException);
}

TEST(TableExceptionTests, RemoveNonExistent) {
    SortedTableOnArr<int, int> table;
    EXPECT_THROW(table.remove(1), TableException);
}

//  UnorderedTreeTable 

class UnorderedTreeTableTest : public ::testing::Test {
protected:
    UnorderedTreeTable<int, std::string> int_str_table;
    UnorderedTreeTable<std::string, double> str_double_table;

    void SetUp() override {
        // Пустые таблицы
    }
};

// Тест создания таблицы
TEST_F(UnorderedTreeTableTest, CreateEmptyTable) {
    EXPECT_TRUE(int_str_table.empty());
    EXPECT_EQ(int_str_table.size(), 0);
    EXPECT_TRUE(str_double_table.empty());
    EXPECT_EQ(str_double_table.size(), 0);
}

// Тест вставки
TEST_F(UnorderedTreeTableTest, InsertRecords) {
    int_str_table.insert(1, "one");
    int_str_table.insert(2, "two");
    int_str_table.insert(3, "three");

    EXPECT_EQ(int_str_table.size(), 3);
    EXPECT_TRUE(int_str_table.contains(1));
    EXPECT_TRUE(int_str_table.contains(2));
    EXPECT_TRUE(int_str_table.contains(3));
    EXPECT_FALSE(int_str_table.contains(4));
}

// Тест вставки дубликата
TEST_F(UnorderedTreeTableTest, InsertDuplicate) {
    int_str_table.insert(1, "one");
    EXPECT_THROW(int_str_table.insert(1, "another_one"), std::runtime_error);
}

// Тест поиска
TEST_F(UnorderedTreeTableTest, Find) {
    int_str_table.insert(1, "one");
    int_str_table.insert(2, "two");

    std::string* val = int_str_table.find(1);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, "one");

    val = int_str_table.find(2);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, "two");

    val = int_str_table.find(3);
    EXPECT_EQ(val, nullptr);
}

// Тест const find
TEST_F(UnorderedTreeTableTest, ConstFind) {
    int_str_table.insert(1, "one");
    const auto& const_table = int_str_table;

    const std::string* val = const_table.find(1);
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, "one");
}

// Тест удаления
TEST_F(UnorderedTreeTableTest, Erase) {
    int_str_table.insert(1, "one");
    int_str_table.insert(2, "two");
    int_str_table.insert(3, "three");

    int_str_table.erase(2);
    EXPECT_EQ(int_str_table.size(), 2);
    EXPECT_TRUE(int_str_table.contains(1));
    EXPECT_FALSE(int_str_table.contains(2));
    EXPECT_TRUE(int_str_table.contains(3));
}

// Тест удаления несуществующего ключа
TEST_F(UnorderedTreeTableTest, EraseNonExistent) {
    int_str_table.insert(1, "one");
    EXPECT_THROW(int_str_table.erase(2), std::runtime_error);
}

// Тест оператора []
TEST_F(UnorderedTreeTableTest, OperatorBrackets) {
    int_str_table.insert(1, "one");
    EXPECT_EQ(int_str_table[1], "one");

    int_str_table[1] = "modified_one";
    EXPECT_EQ(int_str_table[1], "modified_one");

    // Несуществующий ключ должен создать новую запись
    int_str_table[2] = "two";
    EXPECT_EQ(int_str_table[2], "two");
    EXPECT_EQ(int_str_table.size(), 2);
}

// Тест const оператора []
TEST_F(UnorderedTreeTableTest, ConstOperatorBrackets) {
    int_str_table.insert(1, "one");
    const auto& const_table = int_str_table;
    EXPECT_EQ(const_table[1], "one");
}

// Тест очистки
TEST_F(UnorderedTreeTableTest, Clear) {
    int_str_table.insert(1, "one");
    int_str_table.insert(2, "two");
    int_str_table.clear();
    EXPECT_TRUE(int_str_table.empty());
    EXPECT_EQ(int_str_table.size(), 0);
    EXPECT_FALSE(int_str_table.contains(1));
}

// Тест сравнения
TEST_F(UnorderedTreeTableTest, Compare) {
    int_str_table.insert(1, "one");
    int_str_table.insert(2, "two");

    UnorderedTreeTable<int, std::string> other;
    other.insert(1, "one");
    other.insert(2, "two");

    EXPECT_TRUE(int_str_table == other);

    other.insert(3, "three");
    EXPECT_FALSE(int_str_table == other);
}

// Тест getAllRecords
TEST_F(UnorderedTreeTableTest, GetAllRecords) {
    int_str_table.insert(1, "one");
    int_str_table.insert(2, "two");

    auto records = int_str_table.getAllRecords();
    EXPECT_EQ(records.size(), 2);

    bool found_one = false, found_two = false;
    for (const auto& rec : records) {
        if (rec.key == 1 && rec.value == "one") found_one = true;
        if (rec.key == 2 && rec.value == "two") found_two = true;
    }
    EXPECT_TRUE(found_one);
    EXPECT_TRUE(found_two);
}

// Тест обходов дерева таблицы
TEST_F(UnorderedTreeTableTest, Traversals) {
    int_str_table.insert(5, "five");
    int_str_table.insert(1, "one");
    int_str_table.insert(3, "three");
    int_str_table.insert(7, "seven");
    int_str_table.insert(2, "two");
    int_str_table.insert(4, "four");

    // Проверяем, что методы не падают и возвращают непустые строки
    EXPECT_FALSE(int_str_table.traversePreOrder().empty());
    EXPECT_FALSE(int_str_table.traverseInOrder().empty());
    EXPECT_FALSE(int_str_table.traversePostOrder().empty());
    EXPECT_FALSE(int_str_table.traverseLevelOrder().empty());
}

// Тест красивого вывода дерева
TEST_F(UnorderedTreeTableTest, PrintTree) {
    int_str_table.insert(5, "five");
    int_str_table.insert(3, "three");
    int_str_table.insert(7, "seven");

    testing::internal::CaptureStdout();
    int_str_table.printTree();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
}

// Тест с другим типом данных
TEST_F(UnorderedTreeTableTest, StringDoubleTable) {
    str_double_table.insert("pi", 3.14159);
    str_double_table.insert("e", 2.71828);

    EXPECT_EQ(str_double_table.size(), 2);
    EXPECT_TRUE(str_double_table.contains("pi"));
    EXPECT_TRUE(str_double_table.contains("e"));

    double* val = str_double_table.find("pi");
    ASSERT_NE(val, nullptr);
    EXPECT_DOUBLE_EQ(*val, 3.14159);
}

// Тест копирования таблицы
TEST_F(UnorderedTreeTableTest, CopyTable) {
    int_str_table.insert(1, "one");
    int_str_table.insert(2, "two");

    UnorderedTreeTable<int, std::string> copy(int_str_table);
    EXPECT_EQ(copy.size(), 2);
    EXPECT_TRUE(copy.contains(1));
    EXPECT_TRUE(copy.contains(2));
}

// Тест присваивания таблицы
TEST_F(UnorderedTreeTableTest, AssignTable) {
    int_str_table.insert(1, "one");

    UnorderedTreeTable<int, std::string> other;
    other.insert(10, "ten");
    other = int_str_table;

    EXPECT_EQ(other.size(), 1);
    EXPECT_TRUE(other.contains(1));
    EXPECT_FALSE(other.contains(10));
}

//  MAIN 

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}