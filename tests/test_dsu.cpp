#include <gtest/gtest.h>
#include "list_dsu.h"
#include <vector>
#include <algorithm>
#include <random>

class DSUTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::cout << "=== Настройка теста DSU ===\n";
    }

    void TearDown() override {
        std::cout << "=== Завершение теста DSU ===\n\n";
    }
};

// Тест 1: Базовые операции DSU
TEST_F(DSUTest, BasicOperations) {
    std::cout << "Тест 1: Базовые операции DSU\n";

    DSU dsu(10);

    // Проверка начального состояния
    EXPECT_EQ(dsu.count(), 10);

    // Проверка, что каждый элемент в своем множестве
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(dsu.find(i), i);
    }

    // Объединение элементов
    EXPECT_TRUE(dsu.unite(0, 1));
    EXPECT_EQ(dsu.count(), 9);
    EXPECT_TRUE(dsu.connected(0, 1));
    EXPECT_EQ(dsu.find(0), dsu.find(1));

    EXPECT_TRUE(dsu.unite(2, 3));
    EXPECT_EQ(dsu.count(), 8);
    EXPECT_TRUE(dsu.connected(2, 3));

    // Попытка повторного объединения
    EXPECT_FALSE(dsu.unite(0, 1));
    EXPECT_EQ(dsu.count(), 8);

    std::cout << "✓ Базовые операции работают корректно\n";
}

// Тест 2: Транзитивность объединения
TEST_F(DSUTest, Transitivity) {
    std::cout << "\nТест 2: Транзитивность объединения\n";

    DSU dsu(10);

    // Создаем цепочку: 0-1-2-3
    dsu.unite(0, 1);
    dsu.unite(1, 2);
    dsu.unite(2, 3);

    // Проверяем транзитивность
    EXPECT_TRUE(dsu.connected(0, 3));
    EXPECT_TRUE(dsu.connected(1, 3));
    EXPECT_TRUE(dsu.connected(0, 2));

    // Проверяем, что другие элементы не соединены
    EXPECT_FALSE(dsu.connected(0, 4));
    EXPECT_FALSE(dsu.connected(3, 9));

    EXPECT_EQ(dsu.count(), 7); // 10 - 3 объединений = 7 компонент

    std::cout << "✓ Транзитивность работает корректно\n";
}

// Тест 3: Размер компонент
TEST_F(DSUTest, ComponentSize) {
    std::cout << "\nТест 3: Размер компонент\n";

    DSU dsu(10);

    // Объединяем 0, 1, 2
    dsu.unite(0, 1);
    dsu.unite(1, 2);

    // Объединяем 5, 6
    dsu.unite(5, 6);

    // Проверяем размеры компонент
    EXPECT_EQ(dsu.componentSize(0), 3);
    EXPECT_EQ(dsu.componentSize(1), 3);
    EXPECT_EQ(dsu.componentSize(2), 3);

    EXPECT_EQ(dsu.componentSize(5), 2);
    EXPECT_EQ(dsu.componentSize(6), 2);

    // Одиночные элементы
    EXPECT_EQ(dsu.componentSize(3), 1);
    EXPECT_EQ(dsu.componentSize(9), 1);

    std::cout << "✓ Размеры компонент вычисляются правильно\n";
}

// Тест 4: Получение всех компонент
TEST_F(DSUTest, GetAllComponents) {
    std::cout << "\nТест 4: Получение всех компонент\n";

    DSU dsu(10);

    // Создаем 3 компоненты:
    // 1) 0, 1, 2
    // 2) 3, 4, 5
    // 3) 6, 7
    // 4) 8 (один)
    // 5) 9 (один)

    dsu.unite(0, 1);
    dsu.unite(1, 2);

    dsu.unite(3, 4);
    dsu.unite(4, 5);

    dsu.unite(6, 7);

    auto components = dsu.getComponents();

    // Должно быть 5 компонент
    EXPECT_EQ(components.size(), 5);

    // Сортируем компоненты по размеру для удобства проверки
    std::sort(components.begin(), components.end(),
        [](const std::vector<int>& a, const std::vector<int>& b) {
            return a.size() > b.size();
        });

    // Проверяем размеры компонент
    EXPECT_EQ(components[0].size(), 3); // 0,1,2 или 3,4,5
    EXPECT_EQ(components[1].size(), 3); // другая тройка
    EXPECT_EQ(components[2].size(), 2); // 6,7
    EXPECT_EQ(components[3].size(), 1); // 8
    EXPECT_EQ(components[4].size(), 1); // 9

    // Проверяем, что все элементы уникальны
    std::vector<int> allElements;
    for (const auto& comp : components) {
        allElements.insert(allElements.end(), comp.begin(), comp.end());
    }
    std::sort(allElements.begin(), allElements.end());
    allElements.erase(std::unique(allElements.begin(), allElements.end()),
        allElements.end());

    EXPECT_EQ(allElements.size(), 10); // Все 10 элементов

    std::cout << "✓ Все компоненты получены корректно\n";
    std::cout << "  Найдено " << components.size() << " компонент\n";
}

// Тест 5: Задача об островах - пример из задания
TEST_F(DSUTest, IslandsExample) {
    std::cout << "\nТест 5: Задача об островах (пример из задания)\n";

    std::vector<std::vector<int>> grid = {
        {0, 1, 0, 0, 1},
        {0, 1, 1, 0, 1},
        {1, 1, 0, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1}
    };

    IslandCounter counter(grid);

    // Подсчет островов методом DSU
    int islandsDSU = counter.countIslandsDSU();
    int islandsDFS = counter.countIslandsDFS();

    // Ожидаемый результат: 3 острова
    EXPECT_EQ(islandsDSU, 3);
    EXPECT_EQ(islandsDFS, 3);
    EXPECT_EQ(islandsDSU, islandsDFS);

    std::cout << "✓ Пример из задания обработан правильно: "
        << islandsDSU << " острова\n";
}

// Тест 6: Граничные случаи для островов
TEST_F(DSUTest, IslandEdgeCases) {
    std::cout << "\nТест 6: Граничные случаи для островов\n";

    // Тест 6.1: Пустая карта
    {
        std::vector<std::vector<int>> emptyGrid = {};
        IslandCounter emptyCounter(emptyGrid);
        EXPECT_EQ(emptyCounter.countIslandsDSU(), 0);
        std::cout << "  Пустая карта: OK\n";
    }

    // Тест 6.2: Карта без суши
    {
        std::vector<std::vector<int>> waterGrid = {
            {0, 0, 0},
            {0, 0, 0}
        };
        IslandCounter waterCounter(waterGrid);
        EXPECT_EQ(waterCounter.countIslandsDSU(), 0);
        std::cout << "  Карта без суши: OK\n";
    }

    // Тест 6.3: Один большой остров
    {
        std::vector<std::vector<int>> bigIsland = {
            {1, 1, 1},
            {1, 1, 1},
            {1, 1, 1}
        };
        IslandCounter bigCounter(bigIsland);
        EXPECT_EQ(bigCounter.countIslandsDSU(), 1);
        std::cout << "  Один большой остров: OK\n";
    }

    // Тест 6.4: Каждый пиксель - отдельный остров
    {
        std::vector<std::vector<int>> separateIslands = {
            {1, 0, 1},
            {0, 1, 0},
            {1, 0, 1}
        };
        IslandCounter separateCounter(separateIslands);
        EXPECT_EQ(separateCounter.countIslandsDSU(), 5);
        std::cout << "  Отдельные острова (5): OK\n";
    }

    // Тест 6.5: Вертикальная линия
    {
        std::vector<std::vector<int>> verticalLine = {
            {1},
            {1},
            {1},
            {1}
        };
        IslandCounter verticalCounter(verticalLine);
        EXPECT_EQ(verticalCounter.countIslandsDSU(), 1);
        std::cout << "  Вертикальная линия: OK\n";
    }

    // Тест 6.6: Горизонтальная линия
    {
        std::vector<std::vector<int>> horizontalLine = {
            {1, 1, 1, 1}
        };
        IslandCounter horizontalCounter(horizontalLine);
        EXPECT_EQ(horizontalCounter.countIslandsDSU(), 1);
        std::cout << "  Горизонтальная линия: OK\n";
    }
}

// Тест 8: Визуализация островов
TEST_F(DSUTest, IslandVisualization) {
    std::cout << "\nТест 8: Визуализация островов\n";

    std::vector<std::vector<int>> testGrid = {
        {1, 0, 1, 0},
        {1, 1, 0, 1},
        {0, 1, 0, 1},
        {1, 0, 1, 1}
    };

    IslandCounter counter(testGrid);

    // Захватываем вывод
    testing::internal::CaptureStdout();
    counter.printIslands();
    std::string output = testing::internal::GetCapturedStdout();

    // Проверяем, что вывод содержит информацию об островах
    EXPECT_TRUE(output.find("Всего островов:") != std::string::npos);

    int islands = counter.countIslandsDSU();
    EXPECT_TRUE(output.find(std::to_string(islands)) != std::string::npos);

    std::cout << "  Визуализация работает корректно\n";
    std::cout << "  Найдено " << islands << " островов\n";
}

// Тест 9: Случайные тесты для DSU
TEST_F(DSUTest, RandomOperations) {
    std::cout << "\nТест 9: Случайные операции DSU\n";

    const int N = 1000;
    const int OPERATIONS = 5000;

    DSU dsu(N);
    std::vector<std::set<int>> manualSets(N);

    // Инициализация: каждый элемент в своем множестве
    for (int i = 0; i < N; i++) {
        manualSets[i].insert(i);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, N - 1);

    for (int op = 0; op < OPERATIONS; op++) {
        int a = dis(gen);
        int b = dis(gen);

        if (op % 3 == 0) {
            // Операция unite
            bool dsuResult = dsu.unite(a, b);

            // Проверяем вручную
            int rootA = -1, rootB = -1;
            for (int i = 0; i < N; i++) {
                if (manualSets[i].count(a)) rootA = i;
                if (manualSets[i].count(b)) rootB = i;
            }

            bool manualResult = (rootA != rootB);

            if (manualResult) {
                // Объединяем множества
                manualSets[rootA].insert(manualSets[rootB].begin(), manualSets[rootB].end());
                manualSets[rootB].clear();
            }

            EXPECT_EQ(dsuResult, manualResult);

        }
        else if (op % 3 == 1) {
            // Операция connected
            bool dsuConnected = dsu.connected(a, b);

            // Проверяем вручную
            bool manualConnected = false;
            for (const auto& s : manualSets) {
                if (s.count(a) && s.count(b)) {
                    manualConnected = true;
                    break;
                }
            }

            EXPECT_EQ(dsuConnected, manualConnected);
        }
        // else: find операция проверяется в connected
    }

    std::cout << "  " << OPERATIONS << " случайных операций выполнено корректно\n";
}

// Тест 10: Сложные конфигурации островов
TEST_F(DSUTest, ComplexIslandConfigurations) {
    std::cout << "\nТест 10: Сложные конфигурации островов\n";

    struct IslandTestCase {
        std::string name;
        std::vector<std::vector<int>> grid;
        int expectedIslands;
    };

    std::vector<IslandTestCase> testCases = {
        {
            "Кольцо островов",
            {
                {1, 1, 1, 1},
                {1, 0, 0, 1},
                {1, 0, 0, 1},
                {1, 1, 1, 1}
            },
            1  // Одно кольцо - это один остров
        },
        {
            "Спираль",
            {
                {1, 1, 1, 1, 1},
                {0, 0, 0, 0, 1},
                {1, 1, 1, 0, 1},
                {1, 0, 0, 0, 1},
                {1, 1, 1, 1, 1}
            },
            1
        },
        {
            "Крест",
            {
                {0, 1, 0},
                {1, 1, 1},
                {0, 1, 0}
            },
            1
        },
        {
            "Точки касания",
            {
                {1, 0, 1},
                {0, 0, 0},
                {1, 0, 1}
            },
            4  // 4 отдельных острова, не касающихся по диагонали
        },
        {
            "Змейка",
            {
                {1, 1, 1, 0, 0},
                {0, 0, 1, 0, 0},
                {0, 1, 1, 1, 0},
                {0, 1, 0, 1, 0},
                {0, 1, 1, 1, 0}
            },
            1
        }
    };

    for (const auto& testCase : testCases) {
        IslandCounter counter(testCase.grid);
        int islands = counter.countIslandsDSU();

        EXPECT_EQ(islands, testCase.expectedIslands)
            << "Тест не пройден: " << testCase.name
            << ". Ожидалось: " << testCase.expectedIslands
            << ", получено: " << islands;

        std::cout << "  ✓ " << testCase.name << ": "
            << islands << " островов (ожидалось "
            << testCase.expectedIslands << ")\n";
    }
}