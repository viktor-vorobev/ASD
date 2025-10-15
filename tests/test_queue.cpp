#include <gtest/gtest.h>
#include "../lib_queue/queue.h"
#include <string>
#include <stdexcept>

// Тестовый класс для целочисленной очереди
class IntQueueTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Настройка перед каждым тестом
        queue1.enqueue(1);
        queue1.enqueue(2);
        queue1.enqueue(3);
    }

    void TearDown() override {
        // Очистка после каждого теста
    }

    Queue<int> emptyQueue;
    Queue<int> queue1;
};

// Тестовый класс для строковой очереди
class StringQueueTest : public ::testing::Test {
protected:
    void SetUp() override {
        strQueue.enqueue("hello");
        strQueue.enqueue("world");
        strQueue.enqueue("!");
    }

    Queue<std::string> emptyStrQueue;
    Queue<std::string> strQueue;
};

// ТЕСТЫ ДЛЯ ЦЕЛОЧИСЛЕННОЙ ОЧЕРЕДИ


TEST_F(IntQueueTest, DefaultConstructorCreatesEmptyQueue) {
    EXPECT_TRUE(emptyQueue.empty());
    EXPECT_EQ(emptyQueue.size(), 0);
}


TEST_F(IntQueueTest, EnqueueIncreasesSize) {
    EXPECT_EQ(queue1.size(), 3);
    queue1.enqueue(4);
    EXPECT_EQ(queue1.size(), 4);
}


TEST_F(IntQueueTest, DequeueDecreasesSize) {
    queue1.dequeue();
    EXPECT_EQ(queue1.size(), 2);
}


TEST_F(IntQueueTest, FrontReturnsFirstElement) {
    EXPECT_EQ(queue1.front(), 1);
}


TEST_F(IntQueueTest, FIFOOrder) {
    EXPECT_EQ(queue1.front(), 1);
    queue1.dequeue();
    EXPECT_EQ(queue1.front(), 2);
    queue1.dequeue();
    EXPECT_EQ(queue1.front(), 3);
}


TEST_F(IntQueueTest, DequeueEmptyThrowsException) {
    EXPECT_THROW(emptyQueue.dequeue(), std::runtime_error);
}

TEST_F(IntQueueTest, FrontEmptyThrowsException) {
    EXPECT_THROW(emptyQueue.front(), std::runtime_error);
}


TEST_F(IntQueueTest, EmptyMethod) {
    EXPECT_FALSE(queue1.empty());
    EXPECT_TRUE(emptyQueue.empty());
}


TEST_F(IntQueueTest, SizeMethod) {
    EXPECT_EQ(emptyQueue.size(), 0);
    EXPECT_EQ(queue1.size(), 3);
}


TEST_F(IntQueueTest, ClearMakesQueueEmpty) {
    EXPECT_FALSE(queue1.empty());
    queue1.clear();
    EXPECT_TRUE(queue1.empty());
    EXPECT_EQ(queue1.size(), 0);
}


TEST_F(IntQueueTest, CopyConstructor) {
    Queue<int> copiedQueue(queue1);

    // Размеры должны быть одинаковыми
    EXPECT_EQ(queue1.size(), copiedQueue.size());

    // Но это разные объекты
    queue1.dequeue();
    EXPECT_EQ(queue1.size(), 2);
    EXPECT_EQ(copiedQueue.size(), 3);

    // Проверяем, что элементы скопированы правильно
    EXPECT_EQ(copiedQueue.front(), 1);
    copiedQueue.dequeue();
    EXPECT_EQ(copiedQueue.front(), 2);
}


TEST_F(IntQueueTest, AssignmentOperator) {
    Queue<int> assignedQueue;
    assignedQueue = queue1;

    EXPECT_EQ(queue1.size(), assignedQueue.size());

    // Проверяем независимость копий
    queue1.enqueue(4);
    assignedQueue.enqueue(5);

    EXPECT_EQ(queue1.size(), 4);
    EXPECT_EQ(assignedQueue.size(), 4);
}


TEST_F(IntQueueTest, SelfAssignment) {
    queue1 = queue1;
    EXPECT_EQ(queue1.size(), 3);
    EXPECT_EQ(queue1.front(), 1);
}


TEST_F(IntQueueTest, ComplexOperations) {
    Queue<int> queue;

    // Добавляем элементы
    for (int i = 0; i < 10; ++i) {
        queue.enqueue(i);
    }
    EXPECT_EQ(queue.size(), 10);

    // Удаляем несколько элементов
    for (int i = 0; i < 5; ++i) {
        queue.dequeue();
    }
    EXPECT_EQ(queue.size(), 5);
    EXPECT_EQ(queue.front(), 5);

    // Добавляем еще элементы
    queue.enqueue(100);
    queue.enqueue(200);
    EXPECT_EQ(queue.size(), 7);

    // Очищаем
    queue.clear();
    EXPECT_TRUE(queue.empty());
}

// ТЕСТЫ ДЛЯ СТРОКОВОЙ ОЧЕРЕДИ


TEST_F(StringQueueTest, StringOperations) {
    EXPECT_EQ(strQueue.size(), 3);
    EXPECT_EQ(strQueue.front(), "hello");

    strQueue.dequeue();
    EXPECT_EQ(strQueue.front(), "world");

    strQueue.enqueue("test");
    EXPECT_EQ(strQueue.size(), 3);
}


TEST_F(StringQueueTest, EmptyStringQueueThrows) {
    EXPECT_THROW(emptyStrQueue.front(), std::runtime_error);
    EXPECT_THROW(emptyStrQueue.dequeue(), std::runtime_error);
}


TEST_F(StringQueueTest, ClearStringQueue) {
    EXPECT_FALSE(strQueue.empty());
    strQueue.clear();
    EXPECT_TRUE(strQueue.empty());
}

// ТЕСТЫ ДЛЯ ОЧЕРЕДИ С ДРОБНЫМИ ЧИСЛАМИ


TEST(QueueDoubleTest, DoubleOperations) {
    Queue<double> doubleQueue;

    doubleQueue.enqueue(1.5);
    doubleQueue.enqueue(2.7);
    doubleQueue.enqueue(3.14);

    EXPECT_DOUBLE_EQ(doubleQueue.front(), 1.5);
    EXPECT_EQ(doubleQueue.size(), 3);

    doubleQueue.dequeue();
    EXPECT_DOUBLE_EQ(doubleQueue.front(), 2.7);
}

// ТЕСТЫ ГРАНИЧНЫХ СЛУЧАЕВ


TEST(QueueEdgeCases, SingleElement) {
    Queue<int> queue;
    queue.enqueue(42);

    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.size(), 1);
    EXPECT_EQ(queue.front(), 42);

    queue.dequeue();
    EXPECT_TRUE(queue.empty());
}


TEST(QueueEdgeCases, MultipleEnqueueDequeue) {
    Queue<int> queue;

    // Чередуем добавления и удаления
    for (int i = 0; i < 100; ++i) {
        queue.enqueue(i);
        EXPECT_EQ(queue.front(), 0);
    }

    EXPECT_EQ(queue.size(), 100);

    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(queue.front(), i);
        queue.dequeue();
    }

    EXPECT_TRUE(queue.empty());
}


TEST(QueueEdgeCases, MoveBetweenQueues) {
    Queue<int> sourceQueue;
    for (int i = 0; i < 5; ++i) {
        sourceQueue.enqueue(i * 10);
    }

    Queue<int> destQueue = sourceQueue; // Копирование

    // Проверяем, что обе очереди содержат одинаковые данные
    while (!sourceQueue.empty() && !destQueue.empty()) {
        EXPECT_EQ(sourceQueue.front(), destQueue.front());
        sourceQueue.dequeue();
        destQueue.dequeue();
    }

    EXPECT_TRUE(sourceQueue.empty());
    EXPECT_TRUE(destQueue.empty());
}