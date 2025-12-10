#ifndef QUEUE_LIST_BASED_TESTS_H
#define QUEUE_LIST_BASED_TESTS_H

#include "../lib_queue_by_list/queue.h"
#include <gtest/gtest.h>
#include <string>

// Основные тесты для очереди

class QueueTest : public ::testing::Test {
protected:
    void SetUp() override {
        queue.enqueue(1);
        queue.enqueue(2);
        queue.enqueue(3);
    }

    void TearDown() override {
        queue.clear();
    }

    Queue<int> queue;
};

TEST_F(QueueTest, EnqueueAndFront) {
    EXPECT_EQ(queue.front(), 1);
    queue.enqueue(4);
    EXPECT_EQ(queue.front(), 1); // front не меняется при добавлении
    EXPECT_EQ(queue.back(), 4);  // back должен измениться
}

TEST_F(QueueTest, Dequeue) {
    queue.dequeue();
    EXPECT_EQ(queue.front(), 2);

    queue.dequeue();
    EXPECT_EQ(queue.front(), 3);

    queue.dequeue();
    EXPECT_TRUE(queue.empty());
}

TEST_F(QueueTest, SizeAndEmpty) {
    EXPECT_EQ(queue.size(), 3);
    EXPECT_FALSE(queue.empty());

    queue.clear();
    EXPECT_EQ(queue.size(), 0);
    EXPECT_TRUE(queue.empty());
}

TEST_F(QueueTest, EmptyQueueExceptions) {
    Queue<int> emptyQueue;
    EXPECT_THROW(emptyQueue.front(), std::runtime_error);
    EXPECT_THROW(emptyQueue.back(), std::runtime_error);
    EXPECT_THROW(emptyQueue.dequeue(), std::runtime_error);
}

TEST_F(QueueTest, CopyConstructor) {
    Queue<int> copyQueue(queue);
    EXPECT_EQ(copyQueue.size(), 3);
    EXPECT_EQ(copyQueue.front(), 1);

    copyQueue.dequeue();
    EXPECT_EQ(copyQueue.front(), 2);
    EXPECT_EQ(queue.front(), 1); // Оригинал не должен измениться
}

TEST_F(QueueTest, MoveConstructor) {
    Queue<int> movedQueue(std::move(queue));
    EXPECT_EQ(movedQueue.size(), 3);
    EXPECT_EQ(movedQueue.front(), 1);
    EXPECT_EQ(queue.size(), 0); // Оригинал должен быть пуст
    EXPECT_TRUE(queue.empty());
}

TEST_F(QueueTest, AssignmentOperator) {
    Queue<int> assignedQueue;
    assignedQueue = queue;
    EXPECT_EQ(assignedQueue.size(), 3);
    EXPECT_EQ(assignedQueue.front(), 1);

    assignedQueue.dequeue();
    EXPECT_EQ(assignedQueue.front(), 2);
    EXPECT_EQ(queue.front(), 1); // Оригинал не должен измениться
}

TEST_F(QueueTest, MoveAssignmentOperator) {
    Queue<int> movedQueue;
    movedQueue = std::move(queue);
    EXPECT_EQ(movedQueue.size(), 3);
    EXPECT_EQ(movedQueue.front(), 1);
    EXPECT_EQ(queue.size(), 0);
    EXPECT_TRUE(queue.empty());
}

TEST_F(QueueTest, FIFOBehavior) {
    // Проверка поведения FIFO (First-In-First-Out)
    Queue<int> fifoQueue;
    fifoQueue.enqueue(10);
    fifoQueue.enqueue(20);
    fifoQueue.enqueue(30);

    EXPECT_EQ(fifoQueue.front(), 10);
    fifoQueue.dequeue();

    EXPECT_EQ(fifoQueue.front(), 20);
    fifoQueue.dequeue();

    EXPECT_EQ(fifoQueue.front(), 30);
    fifoQueue.dequeue();

    EXPECT_TRUE(fifoQueue.empty());
}

TEST_F(QueueTest, BackAccess) {
    EXPECT_EQ(queue.back(), 3);

    queue.enqueue(4);
    EXPECT_EQ(queue.back(), 4);

    queue.dequeue();
    EXPECT_EQ(queue.back(), 4); // back не меняется при удалении из начала
}

// Тесты с пользовательскими типами данных

struct Person {
    std::string name;
    int age;

    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

TEST(ComplexTypesTest, QueueWithCustomType) {
    Queue<Person> personQueue;
    Person p1{ "Alice", 25 };
    Person p2{ "Bob", 30 };

    personQueue.enqueue(p1);
    personQueue.enqueue(p2);

    EXPECT_EQ(personQueue.front(), p1);
    EXPECT_EQ(personQueue.back(), p2);

    personQueue.dequeue();
    EXPECT_EQ(personQueue.front(), p2);
}

TEST(ComplexTypesTest, QueueWithStrings) {
    Queue<std::string> stringQueue;

    stringQueue.enqueue("Hello");
    stringQueue.enqueue("World");

    EXPECT_EQ(stringQueue.front(), "Hello");
    EXPECT_EQ(stringQueue.back(), "World");

    stringQueue.dequeue();
    EXPECT_EQ(stringQueue.front(), "World");
}

// Тесты перемещающей семантики

TEST(MoveSemanticsTest, MoveEnqueue) {
    Queue<std::string> queue;
    std::string str = "Hello";

    queue.enqueue(std::move(str));
    EXPECT_TRUE(str.empty()); // Проверка, что строка была перемещена
    EXPECT_EQ(queue.front(), "Hello");
}

TEST(MoveSemanticsTest, MoveConstructorPreservesData) {
    Queue<std::string> queue1;
    queue1.enqueue("First");
    queue1.enqueue("Second");

    Queue<std::string> queue2(std::move(queue1));

    EXPECT_TRUE(queue1.empty());
    EXPECT_EQ(queue2.size(), 2);
    EXPECT_EQ(queue2.front(), "First");
    EXPECT_EQ(queue2.back(), "Second");
}

// Тесты итераторов

TEST(IteratorTest, QueueIteration) {
    Queue<int> queue;
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    int sum = 0;
    int count = 0;
    for (int value : queue) {
        sum += value;
        count++;
    }

    EXPECT_EQ(count, 3);
    EXPECT_EQ(sum, 6);
}

TEST(IteratorTest, ConstIterator) {
    Queue<int> queue;
    queue.enqueue(1);
    queue.enqueue(2);

    const Queue<int>& constQueue = queue;

    int sum = 0;
    for (auto it = constQueue.begin(); it != constQueue.end(); ++it) {
        sum += *it;
    }

    EXPECT_EQ(sum, 3);
}

// Тесты очистки

TEST(ClearTest, ClearNonEmptyQueue) {
    Queue<int> queue;
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    EXPECT_EQ(queue.size(), 3);
    queue.clear();
    EXPECT_EQ(queue.size(), 0);
    EXPECT_TRUE(queue.empty());
}

TEST(ClearTest, ClearEmptyQueue) {
    Queue<int> queue;
    queue.clear(); // Не должно вызывать исключений
    EXPECT_TRUE(queue.empty());
}

// Тесты find и contains


TEST(SearchTest, Contains) {
    Queue<int> queue;
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    EXPECT_TRUE(queue.contains(1));
    EXPECT_TRUE(queue.contains(2));
    EXPECT_TRUE(queue.contains(3));
    EXPECT_FALSE(queue.contains(4));
}

TEST(SearchTest, Find) {
    Queue<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    EXPECT_EQ(queue.find(10), 0);
    EXPECT_EQ(queue.find(20), 1);
    EXPECT_EQ(queue.find(30), 2);
    EXPECT_EQ(queue.find(40), -1);
}


// Тесты с одним элементом


TEST(SingleElementTest, OneElementQueue) {
    Queue<int> queue;
    queue.enqueue(42);

    EXPECT_EQ(queue.size(), 1);
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.front(), 42);
    EXPECT_EQ(queue.back(), 42); // front и back одинаковы

    queue.dequeue();
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
}


#endif // QUEUE_LIST_BASED_TESTS_H