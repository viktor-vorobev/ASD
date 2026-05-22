#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "avl_tree.h"
#include "polynom.h"

// БЛОК 1: ТЕСТЫ БАЛАНСИРОВКИ И ЛОГИКИ AVL (int)

TEST(AVLTreeStandardTest, InitialStateEmpty) {
    AVLTree<int, std::string> tree;
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_EQ(tree.find(42), nullptr);
}

TEST(AVLTreeStandardTest, InsertAndEnforceAVLBalancing) {
    AVLTree<int, char> tree;

    // Ситуация жесткого перекоса вправо (должен сработать RR поворот)
    // 1 -> 2 -> 3 превратится в корень 2, левый 1, правый 3
    EXPECT_TRUE(tree.insert(1, 'A'));
    EXPECT_TRUE(tree.insert(2, 'B'));
    EXPECT_TRUE(tree.insert(3, 'C'));

    EXPECT_EQ(tree.size(), 3);
    EXPECT_FALSE(tree.insert(2, 'X')); // Дубликаты ключей запрещены

    std::vector<int> traversedKeys;
    tree.traverseInOrder([&traversedKeys](const int& k, const char& v) {
        traversedKeys.push_back(k);
        });

    // InOrder обход сбалансированного дерева всё равно возвращает отсортированный массив
    std::vector<int> expected = { 1, 2, 3 };
    EXPECT_EQ(traversedKeys, expected);
}

TEST(AVLTreeStandardTest, EraseWithTreeRebalancing) {
    AVLTree<int, std::string> tree;
    tree.insert(10, "Ten");
    tree.insert(5, "Five");
    tree.insert(15, "Fifteen");
    tree.insert(3, "Three");

    // Удаляем элемент 15, дерево должно остаться сбалансированным
    EXPECT_TRUE(tree.erase(15));
    EXPECT_EQ(tree.size(), 3);
    EXPECT_EQ(tree.find(15), nullptr);

    // Удаление несуществующего ключа
    EXPECT_FALSE(tree.erase(99));

    std::vector<int> currentKeys;
    tree.traverseInOrder([&currentKeys](const int& k, const std::string& v) {
        currentKeys.push_back(k);
        });
    std::vector<int> expected = { 3, 5, 10 };
    EXPECT_EQ(currentKeys, expected);
}

TEST(AVLTreeStandardTest, RuleOfFiveDeepCopyAndMove) {
    AVLTree<int, int> origin;
    origin.insert(50, 500);
    origin.insert(25, 250);

    // Тест глубокого копирования
    AVLTree<int, int> copyTree(origin);
    EXPECT_EQ(copyTree.size(), origin.size());

    int* copyVal = copyTree.find(25);
    ASSERT_NE(copyVal, nullptr);
    *copyVal = 999; // Меняем данные в копии дерева

    // Данные в оригинальном дереве НЕ должны измениться
    EXPECT_EQ(*origin.find(25), 250);

    // Тест перемещения (Move Semantics)
    AVLTree<int, int> movedTree(std::move(origin));
    EXPECT_EQ(movedTree.size(), 2);
    EXPECT_TRUE(origin.empty()); // Старое дерево обнулилось
}

// БЛОК 2: ИНТЕГРАЦИОННЫЕ ТЕСТЫ С POLYNOM

TEST(AVLTreePolynomIntegration, StoreAndEvaluatePolynoms) {
    // Ключ — ID (int), Значение — Тяжелый объект Polynom
    AVLTree<int, Polynom> polyTree;

    // Вставка временных rvalue-объектов (вызовется эффективная insert(K&&, V&&))
    EXPECT_TRUE(polyTree.insert(101, Polynom("x^2 + 2x - 1")));
    EXPECT_TRUE(polyTree.insert(102, Polynom("3x^3")));
    EXPECT_TRUE(polyTree.insert(100, Polynom("5"))); // константа

    EXPECT_EQ(polyTree.size(), 3);

    // Поиск полинома в дереве поиска по ключу
    Polynom* p = polyTree.find(101);
    ASSERT_NE(p, nullptr);

    // Вычисляем значение полинома "x^2 + 2x - 1" в точке x=3 (y=0, z=0) -> 3^2 + 2*3 - 1 = 14
    EXPECT_DOUBLE_EQ(p->evaluate(3.0, 0.0, 0.0), 14.0);

    // Модификация полинома прямо внутри узла АВЛ-дерева через оператор *=
    *p *= 2.0; // теперь полином равен "2x^2 + 4x - 2"
    EXPECT_DOUBLE_EQ(p->evaluate(3.0, 0.0, 0.0), 28.0);
}

TEST(AVLTreePolynomIntegration, SafeErasePolynomNoLeaks) {
    AVLTree<std::string, Polynom> polyTree;
    polyTree.insert("poly_A", Polynom("x"));
    polyTree.insert("poly_B", Polynom(""));

    // Проверяем, что удаление узла с Polynom вызывает корректную деструкцию списков List<Monom>
    EXPECT_TRUE(polyTree.erase("poly_B"));
    EXPECT_EQ(polyTree.size(), 1);
    EXPECT_EQ(polyTree.find("poly_B"), nullptr);

    // Проверяем оставшийся полином
    Polynom* remaining = polyTree.find("poly_A");
    ASSERT_NE(remaining, nullptr);
    EXPECT_FALSE(remaining->isEmpty());
}

TEST(AVLTreePolynomIntegration, DestructorCascadeStressTest) {
    // Стресс-тест на каскадное удаление памяти дерева и полиномов без Double Free
    {
        AVLTree<int, Polynom> mainTree;
        mainTree.insert(5, Polynom("x^5"));
        mainTree.insert(2, Polynom("x^2"));
        mainTree.insert(8, Polynom("x^8"));

        {
            // Оператор копирующего присваивания дерева
            AVLTree<int, Polynom> temporaryTree;
            temporaryTree = mainTree;
            EXPECT_EQ(temporaryTree.size(), 3);
            // temporaryTree полностью уничтожается здесь, очищая свои внутренние узлы и полиномы
        }

        // Убеждаемся, что mainTree осталось полностью живым и его память не повреждена
        EXPECT_EQ(mainTree.size(), 3);
        Polynom* rootPoly = mainTree.find(5);
        ASSERT_NE(rootPoly, nullptr);
        EXPECT_EQ(rootPoly->evaluate(2.0, 0.0, 0.0), 32.0); // 2^5 = 32
    }
    // mainTree успешно уничтожено
    SUCCEED();
}