#ifndef DSU_H
#define DSU_H

#include <vector>
#include <iostream>
#include <stdexcept>

class DSU {
private:
    std::vector<int> parent;
    std::vector<int> rank;
    std::vector<int> size;
    int components;

public:
    // Конструктор
    DSU(int n) : components(n) {
        parent.resize(n);
        rank.resize(n, 0);
        size.resize(n, 1);

        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // Найти корень с path compression
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // path compression
        }
        return parent[x];
    }

    // Объединить два множества
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY) {
            return false; // уже в одном множестве
        }

        // Union by rank
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
        }
        else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
        }
        else {
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
            rank[rootX]++;
        }

        components--;
        return true;
    }

    // Проверить, в одном ли множестве элементы
    bool connected(int x, int y) {
        return find(x) == find(y);
    }

    // Количество компонент
    int count() const {
        return components;
    }

    // Размер компоненты
    int componentSize(int x) {
        int root = find(x);
        return size[root];
    }

    // Получить все компоненты
    std::vector<std::vector<int>> getComponents() {
        int n = parent.size();
        std::vector<std::vector<int>> componentsMap(n);

        for (int i = 0; i < n; i++) {
            int root = find(i);
            componentsMap[root].push_back(i);
        }

        // Удаляем пустые компоненты
        std::vector<std::vector<int>> result;
        for (const auto& comp : componentsMap) {
            if (!comp.empty()) {
                result.push_back(comp);
            }
        }

        return result;
    }
};

// Класс для решения задачи об островах
class IslandCounter {
private:
    std::vector<std::vector<int>> grid;
    int rows;
    int cols;

    // Преобразование 2D координат в 1D индекс для DSU
    int toIndex(int r, int c) const {
        return r * cols + c;
    }

    // Проверка валидности координат
    bool isValid(int r, int c) const {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    }

public:
    IslandCounter(const std::vector<std::vector<int>>& inputGrid)
        : grid(inputGrid) {
        if (grid.empty() || grid[0].empty()) {
            rows = cols = 0;
            return;
        }
        rows = grid.size();
        cols = grid[0].size();
    }

    // Метод 1: Подсчет островов с использованием DSU
    int countIslandsDSU() {
        if (rows == 0 || cols == 0) return 0;

        // Создаем DSU для всех ячеек
        DSU dsu(rows * cols);
        int waterCells = 0;

        // Проходим по всем ячейкам
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] == 0) {
                    waterCells++;
                    continue;
                }

                int currentIdx = toIndex(r, c);

                // Проверяем соседей (только вверх и влево, чтобы не дублировать)
                // Соседи: вверх и влево (так мы покрываем все связи)

                // Сосед сверху
                if (r > 0 && grid[r - 1][c] == 1) {
                    int topIdx = toIndex(r - 1, c);
                    dsu.unite(currentIdx, topIdx);
                }

                // Сосед слева
                if (c > 0 && grid[r][c - 1] == 1) {
                    int leftIdx = toIndex(r, c - 1);
                    dsu.unite(currentIdx, leftIdx);
                }
            }
        }

        // Подсчитываем уникальные корни для суши
        std::vector<bool> isRoot(rows * cols, false);
        int islandCount = 0;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] == 1) {
                    int idx = toIndex(r, c);
                    int root = dsu.find(idx);

                    if (!isRoot[root]) {
                        isRoot[root] = true;
                        islandCount++;
                    }
                }
            }
        }

        return islandCount;
    }

    // Метод 2: DFS для сравнения
    void dfs(int r, int c, std::vector<std::vector<bool>>& visited) {
        if (!isValid(r, c) || grid[r][c] == 0 || visited[r][c]) {
            return;
        }

        visited[r][c] = true;

        // Рекурсивно посещаем соседей (4 направления)
        dfs(r - 1, c, visited); // вверх
        dfs(r + 1, c, visited); // вниз
        dfs(r, c - 1, visited); // влево
        dfs(r, c + 1, visited); // вправо
    }

    int countIslandsDFS() {
        if (rows == 0 || cols == 0) return 0;

        std::vector<std::vector<bool>> visited(rows,
            std::vector<bool>(cols, false));
        int islandCount = 0;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] == 1 && !visited[r][c]) {
                    dfs(r, c, visited);
                    islandCount++;
                }
            }
        }

        return islandCount;
    }

    // Вывод карты островов
    void printIslands() {
        if (rows == 0 || cols == 0) return;

        // Создаем DSU и объединяем острова
        DSU dsu(rows * cols);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] == 0) continue;

                int currentIdx = toIndex(r, c);

                if (r > 0 && grid[r - 1][c] == 1) {
                    dsu.unite(currentIdx, toIndex(r - 1, c));
                }
                if (c > 0 && grid[r][c - 1] == 1) {
                    dsu.unite(currentIdx, toIndex(r, c - 1));
                }
            }
        }

        // Назначаем ID каждому острову
        std::vector<int> islandId(rows * cols, -1);
        int nextId = 1;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] == 1) {
                    int idx = toIndex(r, c);
                    int root = dsu.find(idx);

                    if (islandId[root] == -1) {
                        islandId[root] = nextId++;
                    }

                    std::cout << islandId[root] << " ";
                }
                else {
                    std::cout << ". ";
                }
            }
            std::cout << std::endl;
        }

        std::cout << "\nВсего островов: " << (nextId - 1) << std::endl;
    }

    // Статический метод для тестирования
    static void testExample() {
        std::vector<std::vector<int>> grid = {
            {0, 1, 0, 0, 1},
            {0, 1, 1, 0, 1},
            {1, 1, 0, 1, 1},
            {0, 0, 0, 0, 1},
            {1, 0, 1, 1, 1}
        };

        IslandCounter counter(grid);

        std::cout << "Карта островов:\n";
        counter.printIslands();

        int islandsDSU = counter.countIslandsDSU();
        int islandsDFS = counter.countIslandsDFS();

        std::cout << "\nРезультаты:\n";
        std::cout << "Метод DSU: " << islandsDSU << " островов\n";
        std::cout << "Метод DFS: " << islandsDFS << " островов\n";

        if (islandsDSU == islandsDFS) {
            std::cout << "✓ Оба метода дали одинаковый результат!\n";
        }
    }
};

#endif // DSU_H