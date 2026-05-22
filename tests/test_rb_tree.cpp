#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <stdexcept>
#include "rb_tree.h"
#include "polynom.h"

// БЛОК 1: ТЕСТЫ БАЗОВОЙ ЛОГИКИ RB-ДЕРЕВА

TEST(RBTreeStandardTest, InitialState) {
    RBTree<int, std::string> tree;
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_EQ(tree.find(10), nullptr);
}

TEST(RBTreeStandardTest, InsertAndTraverse) {
    RBTree<int, char> tree;

    EXPECT_TRUE(tree.insert(10, 'A'));
    EXPECT_TRUE(tree.insert(20, 'B'));
    EXPECT_TRUE(tree.insert(5, 'C'));

    // Дубликаты запрещены
    EXPECT_FALSE(tree.insert(10, 'X'));
    EXPECT_EQ(tree.size(), 3);

    std::vector<int> keys;
    tree.traverseInOrder([&keys](const int& k, const char& v) {
        keys.push_back(k);
        });

    // InOrder обход всегда должен давать отсортированный результат
    std::vector<int> expected = { 5, 10, 20 };
    EXPECT_EQ(keys, expected);
}

TEST(RBTreeStandardTest, RuleOfFiveDeepCopyAndParentPointers) {
    RBTree<int, int> origin;
    origin.insert(50, 500);
    origin.insert(25, 250);
    origin.insert(75, 750);

    // Тест копирования (copyHelper должен корректно восстановить parent)
    RBTree<int, int> copied(origin);
    EXPECT_EQ(copied.size(), origin.size());

    int* valInCopy = copied.find(25);
    ASSERT_NE(valInCopy, nullptr);
    *valInCopy = 999;

    // Проверяем независимость копии от оригинала
    EXPECT_EQ(*origin.find(25), 250);

    // Добавляем элемент в копию, чтобы спровоцировать балансировку.
    // Если указатели parent были скопированы криво, здесь будет SegFault.
    EXPECT_TRUE(copied.insert(10, 100));
    EXPECT_EQ(copied.size(), 4);

    // Тест перемещения (Move Semantics)
    RBTree<int, int> moved(std::move(origin));
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(origin.empty());
}

TEST(RBTreeStandardTest, EraseAndMaintainProperties) {
    RBTree<int, std::string> tree;

    // Вставляем элементы, провоцируя ребалансировку
    tree.insert(20, "Twenty");
    tree.insert(10, "Ten");
    tree.insert(30, "Thirty");
    tree.insert(5, "Five");
    tree.insert(15, "Fifteen");

    EXPECT_EQ(tree.size(), 5);

    // Удаляем листовой узел
    EXPECT_TRUE(tree.erase(5));
    EXPECT_EQ(tree.find(5), nullptr);
    EXPECT_EQ(tree.size(), 4);

    // Удаляем узел с одним потомком (или корень)
    EXPECT_TRUE(tree.erase(20));
    EXPECT_EQ(tree.find(20), nullptr);
    EXPECT_EQ(tree.size(), 3);

    // Обход InOrder должен всё ещё возвращать отсортированные данные
    std::vector<int> expected = { 10, 15, 30 };
    std::vector<int> actual;
    tree.traverseInOrder([&actual](const int& k, const std::string& v) {
        actual.push_back(k);
        });

    EXPECT_EQ(actual, expected);

    // Удаление несуществующего ключа
    EXPECT_FALSE(tree.erase(999));
}

// БЛОК 2: ИНТЕГРАЦИОННЫЕ ТЕСТЫ С POLYNOM


TEST(RBTreePolynomIntegration, MoveInsertAndEvaluate) {
    RBTree<int, Polynom> polyTree;

    // Использование std::move внутри insert()
    EXPECT_TRUE(polyTree.insert(1, Polynom("x^2 + 2x + 1")));
    EXPECT_TRUE(polyTree.insert(2, Polynom("5x^3")));

    EXPECT_EQ(polyTree.size(), 2);

    Polynom* p = polyTree.find(1);
    ASSERT_NE(p, nullptr);

    // Проверка математики: при x=2, "x^2 + 2x + 1" = 4 + 4 + 1 = 9
    EXPECT_DOUBLE_EQ(p->evaluate(2.0, 0.0, 0.0), 9.0);

    // Модификация (оператор *=)
    *p *= 2.0; // "2x^2 + 4x + 2"
    EXPECT_DOUBLE_EQ(p->evaluate(2.0, 0.0, 0.0), 18.0);
}

TEST(RBTreePolynomIntegration, MemoryLeakPreventionTest) {
    // Этот тест проверяет каскадное удаление памяти без Double Free.
    // Ошибки здесь проявятся как падения (Crash) при выходе из скобок.
    {
        RBTree<int, Polynom> mainTree;
        for (int i = 0; i < 10; ++i) {
            // Динамически вставляем 10 полиномов
            mainTree.insert(i, Polynom(std::to_string(i) + "x"));
        }

        EXPECT_EQ(mainTree.size(), 10);

        {
            // Создаем копию дерева
            RBTree<int, Polynom> tempTree = mainTree;
            EXPECT_EQ(tempTree.size(), 10);

            // tempTree выходит из области видимости и вызывает свой деструктор.
            // Если clearHelper не работает, здесь утечет память.
        }

        // Оригинальное дерево должно выжить и сохранить данные
        EXPECT_EQ(mainTree.size(), 10);
        Polynom* p = mainTree.find(5);
        ASSERT_NE(p, nullptr);
        EXPECT_DOUBLE_EQ(p->evaluate(1.0, 0.0, 0.0), 5.0); // "5x" при x=1
    }
    // mainTree безопасно уничтожено
    SUCCEED();
}

TEST(RBTreePolynomIntegration, ErasePolynomNoLeaks) {
    RBTree<int, Polynom> tree;
    tree.insert(1, Polynom("x^2 + x"));
    tree.insert(2, Polynom("3x"));

    EXPECT_EQ(tree.size(), 2);

    // При вызове erase сработает "delete z" внутри RBTree, 
    // что вызовет деструктор Polynom и безопасно очистит память List<Monom>.
    EXPECT_TRUE(tree.erase(1));
    EXPECT_EQ(tree.size(), 1);
    EXPECT_EQ(tree.find(1), nullptr);

    // Оставшийся узел не должен повредиться
    Polynom* p = tree.find(2);
    ASSERT_NE(p, nullptr);
    EXPECT_DOUBLE_EQ(p->evaluate(4.0, 0.0, 0.0), 12.0);
}