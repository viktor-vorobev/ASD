#include <gtest/gtest.h>
#include "list_dsu.h"
#include <vector>
#include <algorithm>
//Тесты для класса DSU 

TEST(DSU, Constructor) {
    DSU dsu(10);
    EXPECT_EQ(dsu.count(), 10);

    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(dsu.find(i), i);
        EXPECT_EQ(dsu.componentSize(i), 1);
    }
}

TEST(DSU, UnionAndFind) {
    DSU dsu(5);

    // Изначально все элементы раздельны
    EXPECT_FALSE(dsu.connected(0, 1));
    EXPECT_EQ(dsu.count(), 5);

    // Объединяем 0 и 1
    EXPECT_TRUE(dsu.unite(0, 1));
    EXPECT_TRUE(dsu.connected(0, 1));
    EXPECT_EQ(dsu.count(), 4);
    EXPECT_EQ(dsu.find(0), dsu.find(1));
    EXPECT_EQ(dsu.componentSize(0), 2);

    // Повторное объединение должно вернуть false
    EXPECT_FALSE(dsu.unite(0, 1));
    EXPECT_EQ(dsu.count(), 4);
}

TEST(DSU, MultipleUnions) {
    DSU dsu(10);

    // Объединяем в группы: {0,1,2}, {3,4}, {5,6,7,8}, {9}
    dsu.unite(0, 1);
    dsu.unite(1, 2);
    dsu.unite(3, 4);
    dsu.unite(5, 6);
    dsu.unite(6, 7);
    dsu.unite(7, 8);

    // Проверяем связи внутри групп
    EXPECT_TRUE(dsu.connected(0, 2));
    EXPECT_TRUE(dsu.connected(3, 4));
    EXPECT_TRUE(dsu.connected(5, 8));

    // Проверяем отсутствие связей между группами
    EXPECT_FALSE(dsu.connected(0, 3));
    EXPECT_FALSE(dsu.connected(1, 5));
    EXPECT_FALSE(dsu.connected(4, 9));

    // Проверяем размеры компонент
    EXPECT_EQ(dsu.componentSize(0), 3);
    EXPECT_EQ(dsu.componentSize(3), 2);
    EXPECT_EQ(dsu.componentSize(5), 4);
    EXPECT_EQ(dsu.componentSize(9), 1);

    // Проверяем количество компонент
    EXPECT_EQ(dsu.count(), 4);
}

TEST(DSU, PathCompression) {
    DSU dsu(100);

    // Создаем длинную цепочку
    for (int i = 0; i < 99; i++) {
        dsu.unite(i, i + 1);
    }

    // После path compression все find должны быть быстрыми
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(dsu.find(i), dsu.find(0));
    }

    EXPECT_TRUE(dsu.connected(0, 99));
    EXPECT_EQ(dsu.count(), 1);
}

TEST(DSU, GetComponents) {
    DSU dsu(8);

    // Создаем компоненты: {0,1,2}, {3,4}, {5,6,7}
    dsu.unite(0, 1);
    dsu.unite(1, 2);
    dsu.unite(3, 4);
    dsu.unite(5, 6);
    dsu.unite(6, 7);

    auto components = dsu.getComponents();

    EXPECT_EQ(components.size(), 3);

    // Проверяем содержимое компонент
    std::vector<std::vector<int>> expected = {
        {0, 1, 2},
        {3, 4},
        {5, 6, 7}
    };

    // Сортируем для сравнения
    for (auto& comp : components) {
        std::sort(comp.begin(), comp.end());
    }
    std::sort(components.begin(), components.end());

    for (size_t i = 0; i < components.size(); i++) {
        EXPECT_EQ(components[i], expected[i]);
    }
}

// ==================== Тесты для класса IslandCounter ====================

TEST(IslandCounter, EmptyGrid) {
    std::vector<std::vector<int>> emptyGrid = {};
    IslandCounter counter(emptyGrid);

    EXPECT_EQ(counter.countIslandsDSU(), 0);
    EXPECT_EQ(counter.countIslandsDFS(), 0);
}

TEST(IslandCounter, SingleCellWater) {
    std::vector<std::vector<int>> grid = { {0} };
    IslandCounter counter(grid);

    EXPECT_EQ(counter.countIslandsDSU(), 0);
    EXPECT_EQ(counter.countIslandsDFS(), 0);
}

TEST(IslandCounter, SingleCellLand) {
    std::vector<std::vector<int>> grid = { {1} };
    IslandCounter counter(grid);

    EXPECT_EQ(counter.countIslandsDSU(), 1);
    EXPECT_EQ(counter.countIslandsDFS(), 1);
}

TEST(IslandCounter, AllWater) {
    std::vector<std::vector<int>> grid = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    IslandCounter counter(grid);

    EXPECT_EQ(counter.countIslandsDSU(), 0);
    EXPECT_EQ(counter.countIslandsDFS(), 0);
}

TEST(IslandCounter, AllLand) {
    std::vector<std::vector<int>> grid = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    IslandCounter counter(grid);

    EXPECT_EQ(counter.countIslandsDSU(), 1);
    EXPECT_EQ(counter.countIslandsDFS(), 1);
}

TEST(IslandCounter, SingleIsland) {
    std::vector<std::vector<int>> grid = {
        {1, 1, 0},
        {1, 1, 0},
        {0, 0, 0}
    };
    IslandCounter counter(grid);

    EXPECT_EQ(counter.countIslandsDSU(), 1);
    EXPECT_EQ(counter.countIslandsDFS(), 1);
}

TEST(IslandCounter, MultipleIslands) {
    std::vector<std::vector<int>> grid = {
        {1, 0, 1},
        {0, 1, 0},
        {1, 0, 1}
    };
    IslandCounter counter(grid);

    EXPECT_EQ(counter.countIslandsDSU(), 5);
    EXPECT_EQ(counter.countIslandsDFS(), 5);
}

TEST(IslandCounter, ComplexIslands) {
    std::vector<std::vector<int>> grid = {
        {1, 1, 0, 0, 0},
        {1, 1, 0, 0, 1},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1},
        {1, 0, 0, 1, 1}
    };
    IslandCounter counter(grid);

    EXPECT_EQ(counter.countIslandsDSU(), 4);
    EXPECT_EQ(counter.countIslandsDFS(), 4);
}

TEST(IslandCounter, ExampleFromClass) {
    std::vector<std::vector<int>> grid = {
        {0, 1, 0, 0, 1},
        {0, 1, 1, 0, 1},
        {1, 1, 0, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1}
    };
    IslandCounter counter(grid);

    int dsuResult = counter.countIslandsDSU();
    int dfsResult = counter.countIslandsDFS();

    EXPECT_EQ(dsuResult, dfsResult);
    EXPECT_EQ(dsuResult, 3); // По визуальному анализу должно быть 3 острова
}

TEST(IslandCounter, LargeGrid) {
    // Создаем сетку 10x10 с паттерном
    std::vector<std::vector<int>> grid(10, std::vector<int>(10, 0));

    // Создаем несколько островов
    for (int i = 0; i < 10; i += 3) {
        for (int j = 0; j < 10; j += 3) {
            if (i < 8 && j < 8) {
                grid[i][j] = 1;
                grid[i][j + 1] = 1;
                grid[i + 1][j] = 1;
                grid[i + 1][j + 1] = 1;
            }
        }
    }

    IslandCounter counter(grid);

    // Должно быть 9 островов 2x2
    int dsuResult = counter.countIslandsDSU();
    int dfsResult = counter.countIslandsDFS();

    EXPECT_EQ(dsuResult, dfsResult);
    EXPECT_EQ(dsuResult, 9);
}

TEST(IslandCounter, ConsistencyBetweenMethods) {
    // Тест на согласованность методов
    std::vector<std::vector<int>> grid = {
        {1, 0, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 0, 0, 1},
        {1, 0, 1, 0, 1},
        {0, 1, 1, 1, 0}
    };

    IslandCounter counter(grid);

    int dsuResult = counter.countIslandsDSU();
    int dfsResult = counter.countIslandsDFS();

    // Основное требование: оба метода должны давать одинаковый результат
    EXPECT_EQ(dsuResult, dfsResult);
}

TEST(IslandCounter, PrintIslandsOutput) {
    // Этот тест проверяет, что метод printIslands не падает
    // (мы не проверяем вывод, только отсутствие исключений)
    std::vector<std::vector<int>> grid = {
        {1, 0, 1},
        {0, 1, 0},
        {1, 0, 1}
    };

    IslandCounter counter(grid);

    EXPECT_NO_THROW(counter.printIslands());
}

TEST(IslandCounter, TestExampleMethod) {
    // Проверяем, что статический метод теста работает без исключений
    EXPECT_NO_THROW(IslandCounter::testExample());
}

// ==================== Тесты граничных случаев ====================

TEST(IslandCounter, OneRowGrid) {
    std::vector<std::vector<int>> grid = {
        {1, 0, 1, 1, 0, 1}
    };
    IslandCounter counter(grid);

    // Должно быть 3 острова
    EXPECT_EQ(counter.countIslandsDSU(), 3);
    EXPECT_EQ(counter.countIslandsDFS(), 3);
}

TEST(IslandCounter, OneColumnGrid) {
    std::vector<std::vector<int>> grid = {
        {1},
        {0},
        {1},
        {1},
        {0},
        {1}
    };
    IslandCounter counter(grid);

    // Должно быть 3 острова
    EXPECT_EQ(counter.countIslandsDSU(), 3);
    EXPECT_EQ(counter.countIslandsDFS(), 3);
}

TEST(IslandCounter, SnakeIsland) {
    std::vector<std::vector<int>> grid = {
        {1, 1, 1, 1},
        {0, 0, 0, 1},
        {1, 1, 1, 1},
        {1, 0, 0, 0},
        {1, 1, 1, 1}
    };
    IslandCounter counter(grid);

    // Все должно быть одним островом
    EXPECT_EQ(counter.countIslandsDSU(), 1);
    EXPECT_EQ(counter.countIslandsDFS(), 1);
}

// ==================== Тесты производительности (простые) ====================

TEST(DSU, PerformanceUnionFind) {
    const int N = 10000;
    DSU dsu(N);

    // Объединяем все элементы в одну компоненту
    for (int i = 0; i < N - 1; i++) {
        dsu.unite(i, i + 1);
    }

    // Проверяем все элементы
    for (int i = 0; i < N; i++) {
        EXPECT_TRUE(dsu.connected(0, i));
    }

    EXPECT_EQ(dsu.count(), 1);
    EXPECT_EQ(dsu.componentSize(0), N);
}

TEST(IslandCounter, PerformanceLargeGrid) {
    const int SIZE = 100;
    std::vector<std::vector<int>> grid(SIZE, std::vector<int>(SIZE, 0));

    // Шахматная доска - максимум островов
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = (i + j) % 2;
        }
    }

    IslandCounter counter(grid);

    int dsuResult = counter.countIslandsDSU();
    int dfsResult = counter.countIslandsDFS();

    // Каждая черная клетка - отдельный остров
    int expectedIslands = (SIZE * SIZE) / 2 + (SIZE * SIZE) % 2;

    EXPECT_EQ(dsuResult, dfsResult);
    EXPECT_EQ(dsuResult, expectedIslands);
}

