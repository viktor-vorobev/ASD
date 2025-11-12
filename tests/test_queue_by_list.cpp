#include <gtest/gtest.h>
#include "../lib_queue_by_list/queue.h"

// Тесты для очереди
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
    EXPECT_EQ(queue.back(), 4);
    EXPECT_EQ(queue.front(), 1);
}

TEST_F(QueueTest, Dequeue) {
    EXPECT_EQ(queue.dequeue(), 1);
    EXPECT_EQ(queue.dequeue(), 2);
    EXPECT_EQ(queue.dequeue(), 3);
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

    EXPECT_EQ(fifoQueue.dequeue(), 10);
    EXPECT_EQ(fifoQueue.dequeue(), 20);
    EXPECT_EQ(fifoQueue.dequeue(), 30);
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
    EXPECT_EQ(personQueue.dequeue(), p1);
    EXPECT_EQ(personQueue.dequeue(), p2);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}