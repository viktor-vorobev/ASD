#include <gtest/gtest.h>
#include <string>
#include <algorithm>
#include <type_traits>

#include "unsorted_table_arr.h"
#include "sorted_table_arr.h"
#include "unsorted_table_list.h"
#include "unsorted_table_tree.h"
#include "sorted_table_bst.h"
#include "sorted_table_avl.h"
#include "sorted_table_rb.h"
#include "hash_table_chain.h"
#include "hash_table_double.h"
#include "polynom.h"

using namespace testing;

class TableNameGenerator {
public:
    template <typename T>
    static std::string GetName(int) {
        // Базовые типы (int, string)
        if constexpr (std::is_same_v<T, UnsortedTableOnArr<int, std::string>>) return "UnsortedArray";
        else if constexpr (std::is_same_v<T, SortedTableOnArr<int, std::string>>) return "SortedArray";
        else if constexpr (std::is_same_v<T, UnsortedTableOnList<int, std::string>>) return "UnsortedList";
        else if constexpr (std::is_same_v<T, UnsortedTableOnTree<int, std::string>>) return "UnsortedTree";
        else if constexpr (std::is_same_v<T, SortedTableOnBST<int, std::string>>) return "SortedBST";
        else if constexpr (std::is_same_v<T, SortedTableOnAVL<int, std::string>>) return "SortedAVL";
        else if constexpr (std::is_same_v<T, SortedTableOnRB<int, std::string>>) return "SortedRBTree";
        else if constexpr (std::is_same_v<T, HashTableChaining<int, std::string>>) return "HashChaining";
        else if constexpr (std::is_same_v<T, HashTableDouble<int, std::string>>) return "HashDouble";

        // Типы с Полиномами (string, Polynom)
        else if constexpr (std::is_same_v<T, UnsortedTableOnArr<std::string, Polynom>>) return "UnsortedArray";
        else if constexpr (std::is_same_v<T, SortedTableOnArr<std::string, Polynom>>) return "SortedArray";
        else if constexpr (std::is_same_v<T, UnsortedTableOnList<std::string, Polynom>>) return "UnsortedList";
        else if constexpr (std::is_same_v<T, UnsortedTableOnTree<std::string, Polynom>>) return "UnsortedTree";
        else if constexpr (std::is_same_v<T, SortedTableOnBST<std::string, Polynom>>) return "SortedBST";
        else if constexpr (std::is_same_v<T, SortedTableOnAVL<std::string, Polynom>>) return "SortedAVL";
        else if constexpr (std::is_same_v<T, SortedTableOnRB<std::string, Polynom>>) return "SortedRBTree";
        else if constexpr (std::is_same_v<T, HashTableChaining<std::string, Polynom>>) return "HashChaining";
        else if constexpr (std::is_same_v<T, HashTableDouble<std::string, Polynom>>) return "HashDouble";

        else return "UnknownTableType";
    }
};

// 1. basic tests (Типы: <int, std::string>)

template <class T>
class TableBasicTest : public Test {
protected:
    T table;
};

typedef Types<
    UnsortedTableOnArr<int, std::string>,
    SortedTableOnArr<int, std::string>,
    UnsortedTableOnList<int, std::string>,
    UnsortedTableOnTree<int, std::string>,
    SortedTableOnBST<int, std::string>,
    SortedTableOnAVL<int, std::string>,
    SortedTableOnRB<int, std::string>,
    HashTableChaining<int, std::string>,
    HashTableDouble<int, std::string>
> AllTableTypes;

TYPED_TEST_SUITE(TableBasicTest, AllTableTypes, TableNameGenerator);

TYPED_TEST(TableBasicTest, InitialStateIsEmpty) {
    EXPECT_TRUE(this->table.isEmpty());
    EXPECT_EQ(this->table.size(), 0);
}

TYPED_TEST(TableBasicTest, InsertAndFindValidData) {
    this->table.insert(1, "one");
    this->table.insert(2, "two");

    EXPECT_FALSE(this->table.isEmpty());
    EXPECT_EQ(this->table.size(), 2);
    EXPECT_EQ(this->table.find(1), "one");
    EXPECT_EQ(this->table.find(2), "two");
}

TYPED_TEST(TableBasicTest, ContainsReturnsCorrectly) {
    this->table.insert(10, "ten");
    EXPECT_TRUE(this->table.contains(10));
    EXPECT_FALSE(this->table.contains(999));
}

TYPED_TEST(TableBasicTest, ExceptionsOnInvalidOperations) {
    this->table.insert(1, "one");
    EXPECT_THROW(this->table.insert(1, "duplicate"), TableException);
    EXPECT_THROW(this->table.find(99), TableException);
    EXPECT_THROW(this->table.remove(99), TableException);
}

TYPED_TEST(TableBasicTest, RemoveWorksCorrectly) {
    this->table.insert(1, "one");
    this->table.insert(2, "two");
    this->table.insert(3, "three");

    this->table.remove(2);

    EXPECT_EQ(this->table.size(), 2);
    EXPECT_FALSE(this->table.contains(2));
    EXPECT_TRUE(this->table.contains(1));
    EXPECT_TRUE(this->table.contains(3));
}

TYPED_TEST(TableBasicTest, ClearEmptiesTheTable) {
    this->table.insert(1, "one");
    this->table.insert(2, "two");
    this->table.clear();

    EXPECT_TRUE(this->table.isEmpty());
    EXPECT_EQ(this->table.size(), 0);
    EXPECT_FALSE(this->table.contains(1));
}

TYPED_TEST(TableBasicTest, OperatorBracketsReadsAndWrites) {
    this->table[1] = "one";
    EXPECT_EQ(this->table.size(), 1);
    EXPECT_EQ(this->table.find(1), "one");

    this->table[1] = "ONE";
    EXPECT_EQ(this->table.size(), 1);
    EXPECT_EQ(this->table.find(1), "ONE");
}

TYPED_TEST(TableBasicTest, GetKeysAndValues) {
    this->table.insert(1, "one");
    this->table.insert(2, "two");

    auto keys = this->table.getKeys();
    auto values = this->table.getValues();

    EXPECT_EQ(keys.size(), 2);
    EXPECT_EQ(values.size(), 2);

    EXPECT_TRUE(std::find(keys.begin(), keys.end(), 1) != keys.end());
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), 2) != keys.end());

    EXPECT_TRUE(std::find(values.begin(), values.end(), "one") != values.end());
    EXPECT_TRUE(std::find(values.begin(), values.end(), "two") != values.end());
}

TYPED_TEST(TableBasicTest, SerializationAndDeserialization) {
    this->table.insert(1, "one");
    this->table.insert(2, "two");
    this->table.insert(3, "three");

    std::string serializedData = this->table.serialize();

    TypeParam restoredTable;
    restoredTable.deserialize(serializedData);

    EXPECT_EQ(restoredTable.size(), 3);
    EXPECT_EQ(restoredTable.find(1), "one");
    EXPECT_EQ(restoredTable.find(2), "two");
    EXPECT_EQ(restoredTable.find(3), "three");
}

// 2. polynom tests (Типы: <std::string, Polynom>)

template <class T>
class TablePolynomTest : public Test {
protected:
    T table;
};

typedef Types<
    UnsortedTableOnArr<std::string, Polynom>,
    SortedTableOnArr<std::string, Polynom>,
    UnsortedTableOnList<std::string, Polynom>,
    UnsortedTableOnTree<std::string, Polynom>,
    SortedTableOnBST<std::string, Polynom>,
    SortedTableOnAVL<std::string, Polynom>,
    SortedTableOnRB<std::string, Polynom>,
    HashTableChaining<std::string, Polynom>,
    HashTableDouble<std::string, Polynom>
> AllPolynomTableTypes;

TYPED_TEST_SUITE(TablePolynomTest, AllPolynomTableTypes, TableNameGenerator);

TYPED_TEST(TablePolynomTest, InsertAndStorePolynoms) {
    Polynom p1("2x^2y + 3z");
    Polynom p2("5x - 2y^3");

    this->table.insert("PolyA", p1);
    this->table.insert("PolyB", p2);

    EXPECT_EQ(this->table.size(), 2);
    EXPECT_EQ(this->table.find("PolyA"), p1);
    EXPECT_EQ(this->table.find("PolyB"), p2);
}

TYPED_TEST(TablePolynomTest, OperatorBracketsWithPolynom) {
    this->table["P1"] = Polynom("x^2 + y^2");
    EXPECT_TRUE(this->table.contains("P1"));

    this->table["P1"] = Polynom("2x");
    EXPECT_EQ(this->table["P1"], Polynom("2x"));
}

TYPED_TEST(TablePolynomTest, EvaluatePolynomFromTable) {
    this->table.insert("Func", Polynom("2x^2 + 3y - z"));
    double result = this->table.find("Func").evaluate(2.0, 3.0, 5.0);
    EXPECT_DOUBLE_EQ(result, 12.0);
}

TYPED_TEST(TablePolynomTest, MathOperationsBetweenTableElements) {
    this->table["A"] = Polynom("x^2 + 2x");
    this->table["B"] = Polynom("3x + 5");

    Polynom sum = this->table["A"] + this->table["B"];
    EXPECT_EQ(sum, Polynom("x^2 + 5x + 5"));

    Polynom diff = this->table["A"] - this->table["B"];
    EXPECT_EQ(diff, Polynom("x^2 - x - 5"));

    Polynom derivative = this->table["A"].derivative('x');
    EXPECT_EQ(derivative, Polynom("2x + 2"));
}

TYPED_TEST(TablePolynomTest, PolynomSerialization) {
    this->table["Poly1"] = Polynom("3x^2 - y");
    this->table["Poly2"] = Polynom("15z^3");

    std::string serializedData = this->table.serialize();

    TypeParam newTable;
    newTable.deserialize(serializedData);

    EXPECT_EQ(newTable.size(), 2);
    EXPECT_EQ(newTable["Poly1"], Polynom("3x^2 - y"));
    EXPECT_EQ(newTable["Poly2"], Polynom("15z^3"));
}