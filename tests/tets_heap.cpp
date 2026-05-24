#include <gtest/gtest.h>
#include <vector>
#include <stdexcept>
#include "heap.h" 

// 1. Тест начального (пустого) состояния кучи
TEST(MinHeapTest, EmptyHeapBehavior) {
    MinHeap<int> heap;

    // Проверяем, что новая куча действительно пуста
    EXPECT_TRUE(heap.empty());
    EXPECT_EQ(heap.size(), 0);

    // Проверяем, что попытка вызвать pop() из пустой кучи выбрасывает исключение std::underflow_error
    EXPECT_THROW(heap.pop(), std::underflow_error);
}

// 2. Тест вставки и удаления одного элемента
TEST(MinHeapTest, InsertAndPopSingleElement) {
    MinHeap<int> heap;

    heap.insert(42);

    EXPECT_FALSE(heap.empty());
    EXPECT_EQ(heap.size(), 1);

    // pop() должен вернуть вставленный элемент, и куча снова станет пустой
    EXPECT_EQ(heap.pop(), 42);
    EXPECT_TRUE(heap.empty());
    EXPECT_EQ(heap.size(), 0);
}

// 3. Тест сохранения свойства минимальной кучи (сортировка при извлечении)
TEST(MinHeapTest, MaintainsMinHeapProperty) {
    MinHeap<int> heap;

    // Вставляем элементы в хаотичном порядке
    heap.insert(10);
    heap.insert(5);
    heap.insert(30);
    heap.insert(3);
    heap.insert(15);

    EXPECT_EQ(heap.size(), 5);

    // Извлечение должно происходить строго по возрастанию (от меньшего к большему)
    EXPECT_EQ(heap.pop(), 3);
    EXPECT_EQ(heap.pop(), 5);
    EXPECT_EQ(heap.pop(), 10);
    EXPECT_EQ(heap.pop(), 15);
    EXPECT_EQ(heap.pop(), 30);

    EXPECT_TRUE(heap.empty());
}

// 4. Тест корректной обработки дубликатов
TEST(MinHeapTest, HandlesDuplicateElements) {
    MinHeap<int> heap;

    heap.insert(7);
    heap.insert(2);
    heap.insert(7);
    heap.insert(2);
    heap.insert(5);

    EXPECT_EQ(heap.size(), 5);

    EXPECT_EQ(heap.pop(), 2);
    EXPECT_EQ(heap.pop(), 2);
    EXPECT_EQ(heap.pop(), 5);
    EXPECT_EQ(heap.pop(), 7);
    EXPECT_EQ(heap.pop(), 7);
}

// 5. Тесты для функции пирамидальной сортировки heapSort
TEST(HeapSortTest, SortsVariousVectors) {
    // Тест 5.1: Обычный неотсортированный массив
    std::vector<int> unsorted = { 13, 5, 8, 3, 9, 7, 10, 11, 6, 4 };
    std::vector<int> expected = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13 };
    EXPECT_EQ(heapSort(unsorted), expected);

    // Тест 5.2: Пустой массив
    std::vector<int> empty_vec;
    EXPECT_TRUE(heapSort(empty_vec).empty());

    // Тест 5.3: Уже отсортированный массив
    std::vector<int> already_sorted = { 1, 2, 3, 4, 5 };
    EXPECT_EQ(heapSort(already_sorted), already_sorted);

    // Тест 5.4: Массив, отсортированный в обратном порядке
    std::vector<int> reverse_sorted = { 5, 4, 3, 2, 1 };
    std::vector<int> expected_reverse = { 1, 2, 3, 4, 5 };
    EXPECT_EQ(heapSort(reverse_sorted), expected_reverse);
}

// Главная функция для запуска всех тестов Google Test
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}