#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "bst.h"
#include "polynom.h"

// ==========================================
// БЛОК 1: ТЕСТЫ СПЕЦИФИКИ BST (ТИПЫ ИНТ)
// ==========================================

TEST(BSTStandardTest, InitialState) {
    BST<int, std::string> tree;
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_EQ(tree.find(5), nullptr);
}

TEST(BSTStandardTest, InsertMaintainsSearchOrder) {
    BST<int, char> tree;
    // Вставляем элементы вразброс
    EXPECT_TRUE(tree.insert(5, 'A'));
    EXPECT_TRUE(tree.insert(3, 'B'));
    EXPECT_TRUE(tree.insert(7, 'C'));

    // Дубликат ключа в BST вставляться не должен
    EXPECT_FALSE(tree.insert(3, 'D'));
    EXPECT_EQ(tree.size(), 3);

    // InOrder обход поискового дерева ВСЕГДА должен возвращать ключи по возрастанию
    std::vector<int> orderedKeys;
    tree.traverseInOrder([&orderedKeys](const int& key, const char& val) {
        orderedKeys.push_back(key);
        });

    std::vector<int> expected = { 3, 5, 7 };
    EXPECT_EQ(orderedKeys, expected);
}

TEST(BSTStandardTest, EraseNodeScenarios) {
    BST<int, std::string> tree;
    tree.insert(50, "Root");
    tree.insert(30, "Left");
    tree.insert(70, "Right");
    tree.insert(20, "Left-Left");
    tree.insert(40, "Left-Right");

    // 1. Удаление листа (20)
    EXPECT_TRUE(tree.erase(20));
    EXPECT_EQ(tree.find(20), nullptr);
    EXPECT_EQ(tree.size(), 4);

    // 2. Удаление узла с одним потомком (30 -> теперь у него остался только 40)
    EXPECT_TRUE(tree.erase(30));
    EXPECT_EQ(tree.find(30), nullptr);
    EXPECT_NE(tree.find(40), nullptr);

    // 3. Удаление корня дерева с двумя потомками (50)
    EXPECT_TRUE(tree.erase(50));
    EXPECT_EQ(tree.find(50), nullptr);

    // Проверяем, что дерево осталось валидным BST после всех удалений
    std::vector<int> orderedKeys;
    tree.traverseInOrder([&orderedKeys](const int& key, const std::string& val) {
        orderedKeys.push_back(key);
        });

    // Ключи должны остаться отсортированными: 40, 70
    std::vector<int> expected = { 40, 70 };
    EXPECT_EQ(orderedKeys, expected);
}

TEST(BSTStandardTest, DeepCopyAndMoveSemantics) {
    BST<int, int> original;
    original.insert(10, 100);
    original.insert(5, 50);

    // Тест глубокого копирования
    BST<int, int> copied(original);
    EXPECT_EQ(copied.size(), original.size());

    int* valInCopy = copied.find(5);
    ASSERT_NE(valInCopy, nullptr);
    *valInCopy = 999; // меняем значение в копии

    // В оригинале значение не должно измениться
    EXPECT_EQ(*original.find(5), 50);

    // Тест перемещения
    BST<int, int> moved(std::move(original));
    EXPECT_EQ(moved.size(), 2);
    EXPECT_TRUE(original.empty());
}

// ==========================================
// БЛОК 2: ИНТЕГРАЦИОННЫЕ ТЕСТЫ С POLYNOM
// ==========================================

TEST(BSTPolynomIntegration, PolynomAsValue) {
    // Ключ — ID (int), значение — Полином
    BST<int, Polynom> polyTree;

    // Проверяем вставку и работу rvalue/move семантики
    EXPECT_TRUE(polyTree.insert(1, Polynom("x^2 + 5")));
    EXPECT_TRUE(polyTree.insert(2, Polynom("2x - 3")));

    EXPECT_EQ(polyTree.size(), 2);

    // Поиск полинома по ключу
    Polynom* p = polyTree.find(1);
    ASSERT_NE(p, nullptr);

    // Вычисляем значение полинома "x^2 + 5" в точке x=2
    EXPECT_EQ(p->evaluate(2.0, 0.0, 0.0), 9.0);

    // Модифицируем полином внутри BST
    *p *= 2.0; // "2x^2 + 10"
    EXPECT_EQ(p->evaluate(2.0, 0.0, 0.0), 18.0);
}

TEST(BSTPolynomIntegration, ErasePolynomSafely) {
    BST<std::string, Polynom> polyTree;

    polyTree.insert("first", Polynom("x"));
    polyTree.insert("second", Polynom(""));

    // Проверяем удаление узла, содержащего Polynom (проверка корректности вызова деструкторов)
    EXPECT_TRUE(polyTree.erase("second"));
    EXPECT_EQ(polyTree.size(), 1);
    EXPECT_EQ(polyTree.find("second"), nullptr);
}