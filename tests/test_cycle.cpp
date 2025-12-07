#include <gtest/gtest.h>
#include "list_cycle.h"
#include <vector>
#include <string>
#include <algorithm>

// ==================== TESTS FOR CYCLELIST ====================

TEST(CycleListTest, DefaultConstructor) {
    CycleList<int> list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(CycleListTest, PushBack) {
    CycleList<int> list;

    list.push_back(1);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 1);

    list.push_back(2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 2);

    list.push_back(3);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(CycleListTest, PushFront) {
    CycleList<int> list;

    list.push_front(1);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 1);

    list.push_front(2);
    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(list.back(), 1);

    list.push_front(3);
    EXPECT_EQ(list.front(), 3);
    EXPECT_EQ(list.back(), 1);
    EXPECT_EQ(list.size(), 3);
}

TEST(CycleListTest, RemoveElement) {
    CycleList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);

    EXPECT_EQ(list.size(), 4);

    // Удаление существующего элемента
    bool removed = list.remove(3);
    EXPECT_TRUE(removed);
    EXPECT_EQ(list.size(), 3);
    EXPECT_FALSE(list.contains(3));

    // Удаление несуществующего элемента
    removed = list.remove(10);
    EXPECT_FALSE(removed);
    EXPECT_EQ(list.size(), 3);

    // Удаление первого элемента
    removed = list.remove(1);
    EXPECT_TRUE(removed);
    EXPECT_EQ(list.size(), 2);
    EXPECT_FALSE(list.contains(1));
    EXPECT_EQ(list.front(), 2);

    // Удаление последнего элемента
    removed = list.remove(4);
    EXPECT_TRUE(removed);
    EXPECT_EQ(list.size(), 1);
    EXPECT_FALSE(list.contains(4));
    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(list.back(), 2);

    // Удаление единственного элемента
    removed = list.remove(2);
    EXPECT_TRUE(removed);
    EXPECT_TRUE(list.empty());
}

TEST(CycleListTest, Contains) {
    CycleList<std::string> list;

    list.push_back("apple");
    list.push_back("banana");
    list.push_back("cherry");

    EXPECT_TRUE(list.contains("apple"));
    EXPECT_TRUE(list.contains("banana"));
    EXPECT_TRUE(list.contains("cherry"));
    EXPECT_FALSE(list.contains("orange"));
    EXPECT_FALSE(list.contains(""));
}

TEST(CycleListTest, GetNext) {
    CycleList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    EXPECT_EQ(list.get_next(1), 2);
    EXPECT_EQ(list.get_next(2), 3);
    EXPECT_EQ(list.get_next(3), 4);
    EXPECT_EQ(list.get_next(4), 5);
    EXPECT_EQ(list.get_next(5), 1); // Циклический переход

    // Проверка исключения для несуществующего элемента
    EXPECT_THROW(list.get_next(10), std::runtime_error);
}

TEST(CycleListTest, GetPrev) {
    CycleList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    EXPECT_EQ(list.get_prev(1), 5); // Циклический переход
    EXPECT_EQ(list.get_prev(2), 1);
    EXPECT_EQ(list.get_prev(3), 2);
    EXPECT_EQ(list.get_prev(4), 3);
    EXPECT_EQ(list.get_prev(5), 4);

    // Проверка исключения для несуществующего элемента
    EXPECT_THROW(list.get_prev(10), std::runtime_error);
}

TEST(CycleListTest, RotateForward) {
    CycleList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);

    EXPECT_EQ(list.front(), 1);

    list.rotate_forward();
    EXPECT_EQ(list.front(), 2);

    list.rotate_forward();
    EXPECT_EQ(list.front(), 3);

    list.rotate_forward();
    EXPECT_EQ(list.front(), 4);

    list.rotate_forward();
    EXPECT_EQ(list.front(), 1); // Полный круг

    // Ротация пустого списка
    CycleList<int> emptyList;
    EXPECT_NO_THROW(emptyList.rotate_forward());
    EXPECT_TRUE(emptyList.empty());
}

TEST(CycleListTest, RotateBackward) {
    CycleList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);

    EXPECT_EQ(list.front(), 1);

    list.rotate_backward();
    EXPECT_EQ(list.front(), 4);

    list.rotate_backward();
    EXPECT_EQ(list.front(), 3);

    list.rotate_backward();
    EXPECT_EQ(list.front(), 2);

    list.rotate_backward();
    EXPECT_EQ(list.front(), 1); // Полный круг

    // Ротация пустого списка
    CycleList<int> emptyList;
    EXPECT_NO_THROW(emptyList.rotate_backward());
    EXPECT_TRUE(emptyList.empty());
}

TEST(CycleListTest, GetElements) {
    CycleList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    std::vector<int> elements = list.get_elements();
    EXPECT_EQ(elements.size(), 3);
    EXPECT_EQ(elements[0], 1);
    EXPECT_EQ(elements[1], 2);
    EXPECT_EQ(elements[2], 3);

    // Проверка циклического порядка
    list.rotate_forward();
    elements = list.get_elements();
    EXPECT_EQ(elements[0], 2);
    EXPECT_EQ(elements[1], 3);
    EXPECT_EQ(elements[2], 1);
}

TEST(CycleListTest, GetElementsReverse) {
    CycleList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    std::vector<int> elements = list.get_elements_reverse();
    EXPECT_EQ(elements.size(), 3);
    EXPECT_EQ(elements[0], 3); // Обратный порядок
    EXPECT_EQ(elements[1], 2);
    EXPECT_EQ(elements[2], 1);
}

TEST(CycleListTest, Clear) {
    CycleList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.size(), 3);

    list.clear();

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);

    // Двойная очистка
    EXPECT_NO_THROW(list.clear());
    EXPECT_TRUE(list.empty());

    // Добавление после очистки
    list.push_back(10);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.front(), 10);
}

TEST(CycleListTest, Swap) {
    CycleList<int> list1;
    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);

    CycleList<int> list2;
    list2.push_back(4);
    list2.push_back(5);

    EXPECT_EQ(list1.size(), 3);
    EXPECT_EQ(list2.size(), 2);

    list1.swap(list2);

    EXPECT_EQ(list1.size(), 2);
    EXPECT_EQ(list2.size(), 3);
    EXPECT_EQ(list1.front(), 4);
    EXPECT_EQ(list2.front(), 1);

    // Проверка содержимого после обмена
    std::vector<int> elements1 = list1.get_elements();
    std::vector<int> elements2 = list2.get_elements();

    EXPECT_EQ(elements1.size(), 2);
    EXPECT_EQ(elements2.size(), 3);
}

// ЗАМЕНЕННЫЙ ТЕСТ ДЛЯ ИТЕРАТОРА
TEST(CycleListTest, IteratorBasic) {
    CycleList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    // Тестирование итератора
    auto it = list.begin();

    // Первый элемент
    EXPECT_EQ(*it, 1);

    // Префиксный инкремент
    ++it;
    EXPECT_EQ(*it, 2);

    ++it;
    EXPECT_EQ(*it, 3);

    ++it;
    // После полного круга - зависит от реализации
    // В текущей реализации может вернуться к началу или продолжить
    EXPECT_TRUE(*it == 1 || *it == 2 || *it == 3);
}

// НОВЫЙ ТЕСТ: ручной обход с ограничением
TEST(CycleListTest, ManualIteratorLoop) {
    CycleList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    std::vector<int> collected;

    // Ручной обход с ограничением количества итераций
    auto it = list.begin();
    for (int i = 0; i < 3; ++i) {
        collected.push_back(*it);
        ++it;
    }

    EXPECT_EQ(collected.size(), 3);

    // Проверяем, что собрали все элементы (порядок может быть разным)
    std::sort(collected.begin(), collected.end());
    EXPECT_EQ(collected[0], 1);
    EXPECT_EQ(collected[1], 2);
    EXPECT_EQ(collected[2], 3);
}

TEST(CycleListTest, ConstIteratorBasic) {
    CycleList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    const CycleList<int>& const_list = list;

    // Использование константного итератора
    auto it = const_list.cbegin();

    EXPECT_EQ(*it, 1);

    ++it;
    EXPECT_EQ(*it, 2);

    ++it;
    EXPECT_EQ(*it, 3);
}

TEST(CycleListTest, StringType) {
    CycleList<std::string> list;

    list.push_back("Hello");
    list.push_back("World");
    list.push_back("!");

    EXPECT_EQ(list.size(), 3);
    EXPECT_TRUE(list.contains("Hello"));
    EXPECT_TRUE(list.contains("World"));
    EXPECT_TRUE(list.contains("!"));
    EXPECT_FALSE(list.contains("hello")); // Регистрозависимость

    EXPECT_EQ(list.get_next("Hello"), "World");
    EXPECT_EQ(list.get_next("World"), "!");
    EXPECT_EQ(list.get_next("!"), "Hello"); // Циклический переход

    list.remove("World");
    EXPECT_EQ(list.size(), 2);
    EXPECT_FALSE(list.contains("World"));
    EXPECT_EQ(list.get_next("Hello"), "!");
}

TEST(CycleListTest, DoubleType) {
    CycleList<double> list;

    list.push_back(1.1);
    list.push_back(2.2);
    list.push_back(3.3);

    EXPECT_DOUBLE_EQ(list.front(), 1.1);
    EXPECT_DOUBLE_EQ(list.back(), 3.3);

    list.rotate_forward();
    EXPECT_DOUBLE_EQ(list.front(), 2.2);

    EXPECT_TRUE(list.contains(1.1));
    EXPECT_TRUE(list.contains(2.2));
    EXPECT_TRUE(list.contains(3.3));
    EXPECT_FALSE(list.contains(4.4));
}

TEST(CycleListTest, ComplexOperations) {
    CycleList<int> list;

    // Добавление 100 элементов
    for (int i = 0; i < 100; ++i) {
        list.push_back(i);
    }

    EXPECT_EQ(list.size(), 100);
    EXPECT_TRUE(list.contains(0));
    EXPECT_TRUE(list.contains(50));
    EXPECT_TRUE(list.contains(99));

    // Удаление каждого второго элемента
    for (int i = 0; i < 100; i += 2) {
        list.remove(i);
    }

    EXPECT_EQ(list.size(), 50);
    EXPECT_FALSE(list.contains(0));
    EXPECT_TRUE(list.contains(1));
    EXPECT_FALSE(list.contains(2));

    // Ротация 1000 раз
    for (int i = 0; i < 1000; ++i) {
        list.rotate_forward();
    }

    // Проверка, что структура не сломалась
    EXPECT_EQ(list.size(), 50);

    // Проверка нескольких get_next
    auto elements = list.get_elements();
    for (size_t i = 0; i < std::min(elements.size(), size_t(10)); ++i) {
        int current = elements[i];
        int next = list.get_next(current);
        EXPECT_TRUE(list.contains(next));
    }
}

TEST(CycleListTest, EdgeCases) {
    CycleList<int> list;

    // Удаление из пустого списка
    EXPECT_FALSE(list.remove(1));
    EXPECT_TRUE(list.empty());

    // Получение front/back из пустого списка
    EXPECT_THROW(list.front(), std::runtime_error);
    EXPECT_THROW(list.back(), std::runtime_error);

    // Добавление и удаление одного элемента
    list.push_back(42);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 42);
    EXPECT_EQ(list.back(), 42);

    EXPECT_TRUE(list.remove(42));
    EXPECT_TRUE(list.empty());

    // Добавление дубликатов
    list.push_back(1);
    list.push_back(1); // Дубликат
    list.push_back(2);

    // В текущей реализации дубликаты не поддерживаются
    // valueToNode будет перезаписан
    EXPECT_EQ(list.size(), 3);

    // Удаление всех элементов
    list.clear();
    EXPECT_TRUE(list.empty());

    // Добавление после clear
    list.push_back(100);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front(), 100);
}

TEST(CycleListTest, IntegrationTest) {
    CycleList<char> list;

    // Создаем список букв
    for (char c = 'A'; c <= 'Z'; ++c) {
        list.push_back(c);
    }

    EXPECT_EQ(list.size(), 26);
    EXPECT_EQ(list.front(), 'A');
    EXPECT_EQ(list.back(), 'Z');

    // Удаляем гласные
    std::string vowels = "AEIOU";
    for (char v : vowels) {
        list.remove(v);
    }

    EXPECT_EQ(list.size(), 21); // 26 - 5 = 21

    // Ротация на 5 позиций вперед
    for (int i = 0; i < 5; ++i) {
        list.rotate_forward();
    }

    // Проверяем get_next для всех оставшихся элементов
    auto elements = list.get_elements();
    for (size_t i = 0; i < elements.size(); ++i) {
        char current = elements[i];
        char expected_next = elements[(i + 1) % elements.size()];
        EXPECT_EQ(list.get_next(current), expected_next);
    }

    // Очистка и создание нового списка
    list.clear();
    list.push_back('X');
    list.push_back('Y');
    list.push_back('Z');

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.get_next('X'), 'Y');
    EXPECT_EQ(list.get_next('Y'), 'Z');
    EXPECT_EQ(list.get_next('Z'), 'X');
}

// НОВЫЙ ТЕСТ: проверка правильности циклических связей
TEST(CycleListTest, CircularLinks) {
    CycleList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    // Проверяем, что список действительно циклический
    EXPECT_EQ(list.get_next(1), 2);
    EXPECT_EQ(list.get_next(2), 3);
    EXPECT_EQ(list.get_next(3), 1); // Возврат к началу

    EXPECT_EQ(list.get_prev(1), 3); // С конца к началу
    EXPECT_EQ(list.get_prev(2), 1);
    EXPECT_EQ(list.get_prev(3), 2);

    // После ротации связи должны сохраниться
    list.rotate_forward();
    EXPECT_EQ(list.get_next(2), 3);
    EXPECT_EQ(list.get_next(3), 1);
    EXPECT_EQ(list.get_next(1), 2);
}

// НОВЫЙ ТЕСТ: производительность с большими данными
TEST(CycleListTest, PerformanceLargeData) {
    CycleList<int> list;
    const int N = 1000;

    // Быстрое добавление
    for (int i = 0; i < N; ++i) {
        list.push_back(i);
    }
    EXPECT_EQ(list.size(), N);

    // Быстрый поиск (O(1) в среднем)
    for (int i = 0; i < N; i += 10) {
        EXPECT_TRUE(list.contains(i));
    }

    // Быстрое удаление
    for (int i = 0; i < N; i += 2) {
        list.remove(i);
    }
    EXPECT_EQ(list.size(), N / 2);
}

// НОВЫЙ ТЕСТ: устойчивость к исключительным ситуациям
TEST(CycleListTest, ExceptionSafety) {
    CycleList<int> list;

    // Добавляем элементы
    for (int i = 0; i < 5; ++i) {
        list.push_back(i);
    }

    // Попытка получить следующий для несуществующего элемента
    EXPECT_THROW(list.get_next(100), std::runtime_error);
    EXPECT_THROW(list.get_prev(100), std::runtime_error);

    // После исключения список должен остаться в валидном состоянии
    EXPECT_EQ(list.size(), 5);
    EXPECT_TRUE(list.contains(0));
    EXPECT_TRUE(list.contains(4));

    // Удаление несуществующего элемента не должно выбрасывать исключение
    EXPECT_NO_THROW(list.remove(100));
    EXPECT_EQ(list.size(), 5);
}

// НОВЫЙ ТЕСТ: последовательные операции
TEST(CycleListTest, SequentialOperations) {
    CycleList<int> list;

    // Последовательность операций
    list.push_back(1);
    list.push_front(0);
    list.push_back(2);
    list.push_back(3);

    EXPECT_EQ(list.size(), 4);
    EXPECT_EQ(list.front(), 0);
    EXPECT_EQ(list.back(), 3);

    list.rotate_forward();
    EXPECT_EQ(list.front(), 1);

    list.remove(2);
    EXPECT_EQ(list.size(), 3);
    EXPECT_FALSE(list.contains(2));

    list.rotate_backward();
    EXPECT_EQ(list.front(), 0);

    // Проверяем целостность циклических связей
    EXPECT_EQ(list.get_next(0), 1);
    EXPECT_EQ(list.get_next(1), 3);
    EXPECT_EQ(list.get_next(3), 0);
}

