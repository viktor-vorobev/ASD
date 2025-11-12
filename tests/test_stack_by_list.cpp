#include <gtest/gtest.h>
#include "../lib_stack_by_list/stack.h"

// Тесты для стека
class StackTest : public ::testing::Test {
protected:
    void SetUp() override {
        stack.push(1);
        stack.push(2);
        stack.push(3);
    }

    void TearDown() override {
        stack.clear();
    }

    Stack<int> stack;
};

TEST_F(StackTest, PushAndTop) {
    EXPECT_EQ(stack.top(), 3);
    stack.push(4);
    EXPECT_EQ(stack.top(), 4);
}

TEST_F(StackTest, Pop) {
    EXPECT_EQ(stack.pop(), 3);
    EXPECT_EQ(stack.pop(), 2);
    EXPECT_EQ(stack.pop(), 1);
}

TEST_F(StackTest, SizeAndEmpty) {
    EXPECT_EQ(stack.size(), 3);
    EXPECT_FALSE(stack.empty());

    stack.clear();
    EXPECT_EQ(stack.size(), 0);
    EXPECT_TRUE(stack.empty());
}

TEST_F(StackTest, EmptyStackExceptions) {
    Stack<int> emptyStack;
    EXPECT_THROW(emptyStack.top(), std::runtime_error);
    EXPECT_THROW(emptyStack.pop(), std::runtime_error);
}

TEST_F(StackTest, CopyConstructor) {
    Stack<int> copyStack(stack);
    EXPECT_EQ(copyStack.size(), 3);
    EXPECT_EQ(copyStack.top(), 3);

    copyStack.pop();
    EXPECT_EQ(copyStack.top(), 2);
    EXPECT_EQ(stack.top(), 3); // Оригинал не должен измениться
}

TEST_F(StackTest, MoveConstructor) {
    Stack<int> movedStack(std::move(stack));
    EXPECT_EQ(movedStack.size(), 3);
    EXPECT_EQ(movedStack.top(), 3);
    EXPECT_EQ(stack.size(), 0); // Оригинал должен быть пуст
    EXPECT_TRUE(stack.empty());
}

TEST_F(StackTest, AssignmentOperator) {
    Stack<int> assignedStack;
    assignedStack = stack;
    EXPECT_EQ(assignedStack.size(), 3);
    EXPECT_EQ(assignedStack.top(), 3);

    assignedStack.pop();
    EXPECT_EQ(assignedStack.top(), 2);
    EXPECT_EQ(stack.top(), 3); // Оригинал не должен измениться
}

TEST_F(StackTest, MoveAssignmentOperator) {
    Stack<int> movedStack;
    movedStack = std::move(stack);
    EXPECT_EQ(movedStack.size(), 3);
    EXPECT_EQ(movedStack.top(), 3);
    EXPECT_EQ(stack.size(), 0);
    EXPECT_TRUE(stack.empty());
}

TEST_F(StackTest, LIFOBehavior) {
    // Проверка поведения LIFO (Last-In-First-Out)
    Stack<int> lifoStack;
    lifoStack.push(10);
    lifoStack.push(20);
    lifoStack.push(30);

    EXPECT_EQ(lifoStack.pop(), 30);
    EXPECT_EQ(lifoStack.pop(), 20);
    EXPECT_EQ(lifoStack.pop(), 10);
}

// Тесты с пользовательскими типами данных
struct Person {
    std::string name;
    int age;

    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

TEST(ComplexTypesTest, StackWithCustomType) {
    Stack<Person> personStack;
    Person p1{ "Alice", 25 };
    Person p2{ "Bob", 30 };

    personStack.push(p1);
    personStack.push(p2);

    EXPECT_EQ(personStack.top(), p2);
    EXPECT_EQ(personStack.pop(), p2);
    EXPECT_EQ(personStack.pop(), p1);
}