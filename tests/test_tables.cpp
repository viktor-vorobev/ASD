#include <gtest/gtest.h>
#include <string>
#include <map>
#include "monom.cpp"
#include "polynom.cpp"
#include "1_table.cpp"
#include "2_table.cpp"
#include "3_table.cpp"
#include "4_table.h"
#include "5_table.cpp"

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

class TreeTableTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Инициализация перед каждым тестом
    }

    void TearDown() override {
        // Очистка после каждого теста
    }
};

// Тест 1: Создание пустой таблицы
TEST_F(TreeTableTest, EmptyTable) {
    TreeTable<int, std::string> table;

    EXPECT_TRUE(table.isEmpty());
    EXPECT_EQ(0, table.size());
}

// Тест 2: Вставка элементов
TEST_F(TreeTableTest, InsertElements) {
    TreeTable<int, std::string> table;

    table.insert(5, "five");
    table.insert(3, "three");
    table.insert(7, "seven");
    table.insert(1, "one");
    table.insert(4, "four");

    EXPECT_FALSE(table.isEmpty());
    EXPECT_EQ(5, table.size());

    EXPECT_TRUE(table.contains(5));
    EXPECT_TRUE(table.contains(3));
    EXPECT_TRUE(table.contains(7));
    EXPECT_TRUE(table.contains(1));
    EXPECT_TRUE(table.contains(4));
    EXPECT_FALSE(table.contains(2));
}

// Тест 3: Поиск элементов
TEST_F(TreeTableTest, FindElements) {
    TreeTable<int, std::string> table;

    table.insert(5, "five");
    table.insert(3, "three");
    table.insert(7, "seven");

    EXPECT_EQ("five", table.find(5));
    EXPECT_EQ("three", table.find(3));
    EXPECT_EQ("seven", table.find(7));

    EXPECT_THROW(table.find(10), TableException);
}

// Тест 4: Обновление существующих элементов
TEST_F(TreeTableTest, UpdateElements) {
    TreeTable<int, std::string> table;

    table.insert(5, "five");
    table.insert(5, "FIVE");

    EXPECT_EQ(1, table.size());
    EXPECT_EQ("FIVE", table.find(5));
}

// Тест 5: Оператор []
TEST_F(TreeTableTest, OperatorBrackets) {
    TreeTable<int, std::string> table;

    table[5] = "five";
    table[3] = "three";

    EXPECT_EQ("five", table[5]);
    EXPECT_EQ("three", table[3]);
    EXPECT_EQ(2, table.size());

    // Несуществующий ключ должен создать элемент с значением по умолчанию
    std::string defaultValue = table[10];
    EXPECT_EQ("", defaultValue); // Для std::string значение по умолчанию - пустая строка
    EXPECT_EQ(3, table.size());
}

// Тест 6: Удаление листа
TEST_F(TreeTableTest, RemoveLeafNode) {
    TreeTable<int, std::string> table;

    // Создаем дерево
    //       5
    //    /     \
    //   3       7
    //  / \     /
    // 1   4   6
    table.insert(5, "five");
    table.insert(3, "three");
    table.insert(7, "seven");
    table.insert(1, "one");
    table.insert(4, "four");
    table.insert(6, "six");

    EXPECT_EQ(6, table.size());

    // Удаляем лист (4)
    table.remove(4);

    EXPECT_EQ(5, table.size());
    EXPECT_FALSE(table.contains(4));
    EXPECT_TRUE(table.contains(5));
    EXPECT_TRUE(table.contains(3));
    EXPECT_TRUE(table.contains(7));
    EXPECT_TRUE(table.contains(1));
    EXPECT_TRUE(table.contains(6));
}

// Тест 7: Удаление узла с одним ребенком (правым)
TEST_F(TreeTableTest, RemoveNodeWithOneRightChild) {
    TreeTable<int, std::string> table;

    //     5
    //      \
    //       7
    //        \
    //         9
    table.insert(5, "five");
    table.insert(7, "seven");
    table.insert(9, "nine");

    EXPECT_EQ(3, table.size());

    // Удаляем узел 7 (имеет правого ребенка 9)
    table.remove(7);

    EXPECT_EQ(2, table.size());
    EXPECT_FALSE(table.contains(7));
    EXPECT_TRUE(table.contains(5));
    EXPECT_TRUE(table.contains(9));
}

// Тест 8: Удаление узла с одним ребенком (левым)
TEST_F(TreeTableTest, RemoveNodeWithOneLeftChild) {
    TreeTable<int, std::string> table;

    //     9
    //    /
    //   7
    //  /
    // 5
    table.insert(9, "nine");
    table.insert(7, "seven");
    table.insert(5, "five");

    EXPECT_EQ(3, table.size());

    // Удаляем узел 7 (имеет левого ребенка 5)
    table.remove(7);

    EXPECT_EQ(2, table.size());
    EXPECT_FALSE(table.contains(7));
    EXPECT_TRUE(table.contains(9));
    EXPECT_TRUE(table.contains(5));
}

// Тест 9: Удаление узла с двумя детьми
TEST_F(TreeTableTest, RemoveNodeWithTwoChildren) {
    TreeTable<int, std::string> table;

    //       5
    //    /     \
    //   3       7
    //  / \     / \
    // 1   4   6   8
    table.insert(5, "five");
    table.insert(3, "three");
    table.insert(7, "seven");
    table.insert(1, "one");
    table.insert(4, "four");
    table.insert(6, "six");
    table.insert(8, "eight");

    EXPECT_EQ(7, table.size());

    // Удаляем корень (5) - имеет двух детей
    table.remove(5);

    EXPECT_EQ(6, table.size());
    EXPECT_FALSE(table.contains(5));
    EXPECT_TRUE(table.contains(3));
    EXPECT_TRUE(table.contains(7));
    EXPECT_TRUE(table.contains(1));
    EXPECT_TRUE(table.contains(4));
    EXPECT_TRUE(table.contains(6));
    EXPECT_TRUE(table.contains(8));
}

// Тест 10: Удаление корня (единственного узла)
TEST_F(TreeTableTest, RemoveOnlyRoot) {
    TreeTable<int, std::string> table;

    table.insert(42, "answer");
    EXPECT_EQ(1, table.size());

    table.remove(42);
    EXPECT_TRUE(table.isEmpty());
    EXPECT_EQ(0, table.size());
    EXPECT_THROW(table.find(42), TableException);
}

// Тест 11: Удаление несуществующего элемента
TEST_F(TreeTableTest, RemoveNonExistentElement) {
    TreeTable<int, std::string> table;

    table.insert(5, "five");
    table.insert(3, "three");

    EXPECT_THROW(table.remove(10), TableException);
    EXPECT_EQ(2, table.size());
}

// Тест 12: Получение всех ключей
TEST_F(TreeTableTest, GetKeys) {
    TreeTable<int, std::string> table;

    table.insert(5, "five");
    table.insert(3, "three");
    table.insert(7, "seven");
    table.insert(1, "one");
    table.insert(4, "four");
    table.insert(6, "six");
    table.insert(8, "eight");

    std::vector<int> keys = table.getKeys();

    // Ключи должны быть отсортированы (in-order обход)
    EXPECT_EQ(7, keys.size());
    EXPECT_EQ(1, keys[0]);
    EXPECT_EQ(3, keys[1]);
    EXPECT_EQ(4, keys[2]);
    EXPECT_EQ(5, keys[3]);
    EXPECT_EQ(6, keys[4]);
    EXPECT_EQ(7, keys[5]);
    EXPECT_EQ(8, keys[6]);
}

// Тест 13: Получение всех значений
TEST_F(TreeTableTest, GetValues) {
    TreeTable<int, std::string> table;

    table.insert(5, "five");
    table.insert(3, "three");
    table.insert(7, "seven");
    table.insert(1, "one");
    table.insert(4, "four");

    std::vector<std::string> values = table.getValues();

    EXPECT_EQ(5, values.size());
    EXPECT_EQ("one", values[0]);
    EXPECT_EQ("three", values[1]);
    EXPECT_EQ("four", values[2]);
    EXPECT_EQ("five", values[3]);
    EXPECT_EQ("seven", values[4]);
}

// Тест 14: Проверка корректности BST
TEST_F(TreeTableTest, IsValidBST) {
    TreeTable<int, std::string> table;

    table.insert(5, "five");
    table.insert(3, "three");
    table.insert(7, "seven");

    EXPECT_TRUE(table.isValidBST());
}

// Тест 15: Сериализация и десериализация
TEST_F(TreeTableTest, SerializeDeserialize) {
    TreeTable<int, std::string> table1;

    table1.insert(5, "five");
    table1.insert(3, "three");
    table1.insert(7, "seven");
    table1.insert(1, "one");

    std::string data = table1.serialize();
    EXPECT_FALSE(data.empty());

    TreeTable<int, std::string> table2;
    table2.deserialize(data);

    EXPECT_EQ(table1.size(), table2.size());
    EXPECT_TRUE(table2.contains(5));
    EXPECT_TRUE(table2.contains(3));
    EXPECT_TRUE(table2.contains(7));
    EXPECT_TRUE(table2.contains(1));
    EXPECT_EQ("five", table2.find(5));
    EXPECT_EQ("three", table2.find(3));
}

// Тест 16: Копирование таблицы
TEST_F(TreeTableTest, CopyTable) {
    TreeTable<int, std::string> table1;

    table1.insert(5, "five");
    table1.insert(3, "three");
    table1.insert(7, "seven");

    TreeTable<int, std::string> table2 = table1;

    EXPECT_EQ(table1.size(), table2.size());
    EXPECT_TRUE(table2.contains(5));
    EXPECT_TRUE(table2.contains(3));
    EXPECT_TRUE(table2.contains(7));

    // Изменение копии не должно влиять на оригинал
    table2.insert(1, "one");
    EXPECT_EQ(4, table2.size());
    EXPECT_EQ(3, table1.size());
}

// Тест 17: Очистка таблицы
TEST_F(TreeTableTest, ClearTable) {
    TreeTable<int, std::string> table;

    table.insert(5, "five");
    table.insert(3, "three");
    table.insert(7, "seven");

    EXPECT_FALSE(table.isEmpty());
    EXPECT_EQ(3, table.size());

    table.clear();

    EXPECT_TRUE(table.isEmpty());
    EXPECT_EQ(0, table.size());
    EXPECT_THROW(table.find(5), TableException);
}

// Тест 18: Высота дерева
TEST_F(TreeTableTest, TreeHeight) {
    TreeTable<int, std::string> table;

    EXPECT_EQ(0, table.getHeight());

    table.insert(5, "five");
    EXPECT_EQ(1, table.getHeight());

    table.insert(3, "three");
    EXPECT_EQ(2, table.getHeight());

    table.insert(7, "seven");
    EXPECT_EQ(2, table.getHeight());

    table.insert(1, "one");
    EXPECT_EQ(3, table.getHeight());
}

// Тест 19: Работа с разными типами ключей (строка)
TEST_F(TreeTableTest, StringKeys) {
    TreeTable<std::string, int> table;

    table.insert("apple", 5);
    table.insert("banana", 3);
    table.insert("cherry", 7);

    EXPECT_TRUE(table.contains("apple"));
    EXPECT_TRUE(table.contains("banana"));
    EXPECT_TRUE(table.contains("cherry"));
    EXPECT_FALSE(table.contains("date"));

    EXPECT_EQ(5, table.find("apple"));
    EXPECT_EQ(3, table.find("banana"));
    EXPECT_EQ(7, table.find("cherry"));

    table["apple"] = 10;
    EXPECT_EQ(10, table["apple"]);
}

// Тест 20: Работа с разными типами значений (double)
TEST_F(TreeTableTest, DoubleValues) {
    TreeTable<int, double> table;

    table.insert(1, 1.5);
    table.insert(2, 2.7);
    table.insert(3, 3.14);

    EXPECT_DOUBLE_EQ(1.5, table.find(1));
    EXPECT_DOUBLE_EQ(2.7, table.find(2));
    EXPECT_DOUBLE_EQ(3.14, table.find(3));
}

// Тест 21: Последовательная вставка и удаление
TEST_F(TreeTableTest, SequentialInsertRemove) {
    TreeTable<int, std::string> table;

    for (int i = 0; i < 100; i++) {
        table.insert(i, "value_" + std::to_string(i));
    }

    EXPECT_EQ(100, table.size());

    for (int i = 0; i < 100; i++) {
        EXPECT_TRUE(table.contains(i));
    }

    for (int i = 0; i < 100; i += 2) {
        table.remove(i);
    }

    EXPECT_EQ(50, table.size());

    for (int i = 0; i < 100; i++) {
        if (i % 2 == 0) {
            EXPECT_FALSE(table.contains(i));
        }
        else {
            EXPECT_TRUE(table.contains(i));
        }
    }
}

// Тест 22: toString выводит упорядоченные элементы
TEST_F(TreeTableTest, ToStringOrderedOutput) {
    TreeTable<int, std::string> table;

    table.insert(5, "five");
    table.insert(3, "three");
    table.insert(7, "seven");
    table.insert(1, "one");
    table.insert(4, "four");

    std::string output = table.toString();

    // Проверяем порядок: 1, 3, 4, 5, 7
    size_t pos1 = output.find("1 => one");
    size_t pos3 = output.find("3 => three");
    size_t pos4 = output.find("4 => four");
    size_t pos5 = output.find("5 => five");
    size_t pos7 = output.find("7 => seven");

    EXPECT_LT(pos1, pos3);
    EXPECT_LT(pos3, pos4);
    EXPECT_LT(pos4, pos5);
    EXPECT_LT(pos5, pos7);
}

// Тест 23: Исключения при поиске в пустой таблице
TEST_F(TreeTableTest, ExceptionsInEmptyTable) {
    TreeTable<int, std::string> table;

    EXPECT_THROW(table.find(1), TableException);
    EXPECT_THROW(table.remove(1), TableException);
    EXPECT_NO_THROW(table.insert(1, "one"));
}

// Тест 24: Перезапись значения через оператор []
TEST_F(TreeTableTest, OverwriteThroughOperator) {
    TreeTable<int, std::string> table;

    table[1] = "first";
    EXPECT_EQ("first", table[1]);

    table[1] = "second";
    EXPECT_EQ("second", table[1]);
    EXPECT_EQ(1, table.size());
}

//  MAIN 

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}