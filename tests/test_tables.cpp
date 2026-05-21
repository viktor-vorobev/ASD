#include <gtest/gtest.h>
#include "polynom.h"
#include "unsorted_table_arr.h"
#include "sorted_table_arr.h"
#include "unsorted_table_list.h"
#include "unsorted_table_tree.h"
#include "sorted_table_bst.h"
#include "sorted_table_avl.h"
#include "hash_table_chain.h"
#include "hash_table_double.h"
#include "sorted_table_rb.h"

template <typename T>
class TableTest : public ::testing::Test {
protected:
    T table;
};

using FullTableTypes = ::testing::Types<
    UnsortedTableOnArr<std::string, Polynom>,
    SortedTableOnArr<std::string, Polynom>,
    UnsortedTableOnList<std::string, Polynom>,
    UnsortedTableOnTree<std::string, Polynom>,
    SortedTableOnBST<std::string, Polynom>,
    SortedTableOnAVL<std::string, Polynom>,
    HashTableChaining<std::string, Polynom>,
    HashTableDouble<std::string, Polynom>
>;
TYPED_TEST_SUITE(TableTest, FullTableTypes);

TYPED_TEST(TableTest, InsertAndFindPolynom) {
    Polynom p1("2x^2 + y");
    this->table.insert("p1", p1);
    EXPECT_TRUE(this->table.contains("p1"));
    EXPECT_EQ(this->table.find("p1").toString(), "2x^2 + y");
}

TYPED_TEST(TableTest, RemoveKey) {
    this->table.insert("A", Polynom("x"));
    this->table.remove("A");
    EXPECT_FALSE(this->table.contains("A"));
}

TEST(RBTreeTableTest, InsertAndFind) {
    SortedTableOnRB<std::string, Polynom> rbTable;
    rbTable.insert("10", Polynom("x"));
    EXPECT_TRUE(rbTable.contains("10"));
    EXPECT_EQ(rbTable["10"].toString(), "x");
}