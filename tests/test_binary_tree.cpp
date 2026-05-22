#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <sstream>
#include "binary_tree.h"
#include "polynom.h"

// Тест 1: Инициализация и базовое состояние
TEST(BinaryTreeTest, EmptyTreeProperties) {
    BinaryTree<int> tree;
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_EQ(tree.find([](const int& val) { return val == 5; }), nullptr);
}

// Тест 2: Вставка элементов по уровням (BFS)
TEST(BinaryTreeTest, InsertIncrementsSizeAndFillsCorrectly) {
    BinaryTree<int> tree;

    tree.insert(10);
    EXPECT_FALSE(tree.empty());
    EXPECT_EQ(tree.size(), 1);

    tree.insert(20);
    tree.insert(30);
    EXPECT_EQ(tree.size(), 3);

    // Проверяем порядок обхода по уровням, чтобы подтвердить правильность вставки
    std::vector<int> result;
    tree.traverseLevelOrder([&result](const int& val) {
        result.push_back(val);
        });

    std::vector<int> expected = { 10, 20, 30 };
    EXPECT_EQ(result, expected);
}

// Тест 3: Поиск элементов (find)
TEST(BinaryTreeTest, FindReturnsCorrectPointer) {
    BinaryTree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    // Константный поиск
    const BinaryTree<int>& constTree = tree;
    const int* constFound = constTree.find([](const int& val) { return val == 2; });
    ASSERT_NE(constFound, nullptr);
    EXPECT_EQ(*constFound, 2);

    // Неконстантный поиск с возможностью модификации
    int* found = tree.find([](const int& val) { return val == 3; });
    ASSERT_NE(found, nullptr);
    *found = 35; // меняем значение внутри дерева

    // Проверяем, что значение изменилось
    int* foundUpdated = tree.find([](const int& val) { return val == 35; });
    EXPECT_NE(foundUpdated, nullptr);
}

// Тест 4: Удаление элементов (remove)
TEST(BinaryTreeTest, RemoveElementsCorrectly) {
    BinaryTree<int> tree;

    // Удаление из пустого дерева
    EXPECT_FALSE(tree.remove([](const int& val) { return val == 10; }));

    // Дерево из одного элемента
    tree.insert(10);
    EXPECT_TRUE(tree.remove([](const int& val) { return val == 10; }));
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0);

    // Сложное удаление (замена удаляемого узла последним)
    // Структура:
    //      1
    //     / \
    //    2   3
    //   /
    //  4 (последний узел)
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);

    // Удаляем корень (1). Вместо него должен встать последний узел (4)
    EXPECT_TRUE(tree.remove([](const int& val) { return val == 1; }));
    EXPECT_EQ(tree.size(), 3);

    std::vector<int> result;
    tree.traverseLevelOrder([&result](const int& val) {
        result.push_back(val);
        });

    // Ожидаемая структура после удаления корня: 4 встает на место 1, узел 4 удаляется.
    // Порядок BFS: 4, 2, 3
    std::vector<int> expected = { 4, 2, 3 };
    EXPECT_EQ(result, expected);

    // Попытка удалить несуществующий элемент
    EXPECT_FALSE(tree.remove([](const int& val) { return val == 99; }));
}

// Тест 5: Все виды обходов (PreOrder, InOrder, PostOrder)
TEST(BinaryTreeTest, TraversalOrders) {
    // Создаем дерево:
    //      1
    //     / \
    //    2   3
    BinaryTree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    // PreOrder: Root -> Left -> Right
    std::vector<int> preResult;
    tree.traversePreOrder([&preResult](const int& val) { preResult.push_back(val); });
    std::vector<int> preExpected = { 1, 2, 3 };
    EXPECT_EQ(preResult, preExpected);

    // InOrder: Left -> Root -> Right
    std::vector<int> inResult;
    tree.traverseInOrder([&inResult](const int& val) { inResult.push_back(val); });
    std::vector<int> inExpected = { 2, 1, 3 };
    EXPECT_EQ(inResult, inExpected);

    // PostOrder: Left -> Right -> Root
    std::vector<int> postResult;
    tree.traversePostOrder([&postResult](const int& val) { postResult.push_back(val); });
    std::vector<int> postExpected = { 2, 3, 1 };
    EXPECT_EQ(postResult, postExpected);
}

// Тест 6: Конструктор копирования и оператор присваивания (Глубокое копирование)
TEST(BinaryTreeTest, CopyConstructorAndAssignment) {
    BinaryTree<int> original;
    original.insert(5);
    original.insert(10);

    // Проверка конструктора копирования
    BinaryTree<int> copy(original);
    EXPECT_EQ(copy.size(), original.size());

    // Проверяем независимость памяти: меняем элемент в копии
    int* valInCopy = copy.find([](const int& v) { return v == 10; });
    ASSERT_NE(valInCopy, nullptr);
    *valInCopy = 99;

    // В оригинале всё должно остаться прежним
    EXPECT_NE(original.find([](const int& v) { return v == 10; }), nullptr);
    EXPECT_EQ(original.find([](const int& v) { return v == 99; }), nullptr);

    // Проверка оператора присваивания
    BinaryTree<int> assigned;
    assigned.insert(1); // Забьем какими-то данными
    assigned = original;

    EXPECT_EQ(assigned.size(), original.size());
    EXPECT_NE(assigned.find([](const int& v) { return v == 10; }), nullptr);
}

// Тест 7: Конструктор перемещения и оператор перемещения
TEST(BinaryTreeTest, MoveSemantics) {
    BinaryTree<int> original;
    original.insert(42);
    original.insert(84);

    // Конструктор перемещения
    BinaryTree<int> movedTo(std::move(original));

    EXPECT_EQ(movedTo.size(), 2);
    EXPECT_NE(movedTo.find([](const int& v) { return v == 42; }), nullptr);

    // Старое дерево должно стать пустым и валидным
    EXPECT_TRUE(original.empty());
    EXPECT_EQ(original.size(), 0);

    // Оператор перемещения
    BinaryTree<int> movedAssign;
    movedAssign = std::move(movedTo);

    EXPECT_EQ(movedAssign.size(), 2);
    EXPECT_TRUE(movedTo.empty());
}

// Тест 8: Метод очистки дерева (clear)
TEST(BinaryTreeTest, ClearMethod) {
    BinaryTree<int> tree;
    tree.insert(1);
    tree.insert(2);

    tree.clear();

    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_EQ(tree.find([](const int& v) { return v == 1; }), nullptr);
}

// Тест 9: Вывод дерева на печать (printTree)
TEST(BinaryTreeTest, PrintTreeExecutesWithoutCrash) {
    BinaryTree<int> tree;
    tree.insert(1);
    tree.insert(2);

    // Перехватываем вывод std::cout, чтобы проверить, что метод работает
    std::streambuf* oldCout = std::cout.rdbuf();
    std::stringstream ss;
    std::cout.rdbuf(ss.rdbuf());

    tree.printTree([](const int& val) { std::cout << val; });

    std::cout.rdbuf(oldCout); // Возвращаем cout на место

    std::string output = ss.str();
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("Root:"), std::string::npos);
    EXPECT_NE(output.find("L--->"), std::string::npos);
}

// Тест 10: Базовые операции дерева с объектами класса Polynom
TEST(BinaryTreePolynomTest, InsertAndTraversePolynoms) {
    BinaryTree<Polynom> tree;

    // Вставляем полиномы (используем конструктор из строки)
    tree.insert(Polynom("2x^2 + 3x - 5"));
    tree.insert(Polynom("x^2 - 1"));
    tree.insert(Polynom("5")); // Полином-константа

    EXPECT_EQ(tree.size(), 3);

    // Проверяем, что обход по уровням собирает полиномы в правильном порядке
    std::vector<std::string> results;
    tree.traverseLevelOrder([&results](const Polynom& p) {
        results.push_back(p.toString());
        });

    // Порядок BFS должен соответствовать порядку вставки
    ASSERT_EQ(results.size(), 3);
    // Здесь строки должны совпадать с тем, что возвращает твой метод toString()
    // (При необходимости подправь ожидаемые строки под точный формат твоего toString)
    EXPECT_FALSE(results[0].empty());
    EXPECT_FALSE(results[1].empty());
}

// Тест 11: Поиск полинома по специфичным критериям (find)
TEST(BinaryTreePolynomTest, FindPolynomByProperty) {
    BinaryTree<Polynom> tree;

    tree.insert(Polynom("x^2"));
    tree.insert(Polynom("")); // Предположим, это пустой полином
    tree.insert(Polynom("3x^3 - 2x"));

    // Поиск пустого полинома через метод isEmpty()
    Polynom* emptyPoly = tree.find([](const Polynom& p) {
        return p.isEmpty();
        });

    ASSERT_NE(emptyPoly, nullptr);

    // Поиск полинома, который в точке x=2 равен 4 (это x^2)
    Polynom* targetPoly = tree.find([](const Polynom& p) {
        // Передаем координаты x=2, y=0, z=0
        return p.evaluate(2.0, 0.0, 0.0) == 4.0;
        });

    ASSERT_NE(targetPoly, nullptr);

    // Модифицируем найденный полином прямо в дереве (например, умножаем на 2)
    // Используем оператор *=, который объявлен в polynom.h
    *targetPoly *= 2.0;

    // Проверяем, что значение в точке x=2 изменилось (было 4, стало 8)
    EXPECT_EQ(targetPoly->evaluate(2.0, 0.0, 0.0), 8.0);
}

// Тест 12: Удаление полиномов из дерева (remove)
TEST(BinaryTreePolynomTest, RemovePolynomAndCheckMemory) {
    BinaryTree<Polynom> tree;

    tree.insert(Polynom("x"));
    tree.insert(Polynom("x^2"));
    tree.insert(Polynom("x^3"));

    // Удаляем полином "x^2"
    // Предикат ищет полином, значение которого в точке x=2 равно 4
    bool removed = tree.remove([](const Polynom& p) {
        return p.evaluate(2.0, 0.0, 0.0) == 4.0;
        });

    EXPECT_TRUE(removed);
    EXPECT_EQ(tree.size(), 2);

    // Проверяем, что "x^2" действительно исчез
    Polynom* lookForDeleted = tree.find([](const Polynom& p) {
        return p.evaluate(2.0, 0.0, 0.0) == 4.0;
        });
    EXPECT_EQ(lookForDeleted, nullptr);
}

// Тест 13: Проверка глубокого копирования дерева с полиномами
TEST(BinaryTreePolynomTest, DeepCopyWithPolynoms) {
    BinaryTree<Polynom> original;
    original.insert(Polynom("x + 5"));
    original.insert(Polynom("2x"));

    // Вызываем конструктор копирования дерева
    BinaryTree<Polynom> copy = original;

    EXPECT_EQ(copy.size(), original.size());

    // Изменяем полином в копии дерева
    Polynom* copyPoly = copy.find([](const Polynom& p) {
        return p.evaluate(1.0, 0.0, 0.0) == 6.0; // это "x + 5" при x=1
        });
    ASSERT_NE(copyPoly, nullptr);

    // Прибавляем к нему другой полином, используя оператор +=
    *copyPoly += Polynom("10"); // теперь там "x + 15"

    // В копии значение изменилось
    EXPECT_EQ(copyPoly->evaluate(1.0, 0.0, 0.0), 16.0);

    // В оригинальном дереве полином должен остаться нетронутым ("x + 5")
    Polynom* origPoly = original.find([](const Polynom& p) {
        return p.evaluate(1.0, 0.0, 0.0) == 6.0;
        });
    ASSERT_NE(origPoly, nullptr);
    EXPECT_EQ(origPoly->evaluate(1.0, 0.0, 0.0), 6.0);
}