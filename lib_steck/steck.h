#ifndef STACK_H
#define STACK_H

#include <stdexcept>
#include <iostream>

template<typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* topNode;
    size_t stackSize;

public:
    // Конструктор
    Stack() : topNode(nullptr), stackSize(0) {}

    // Деструктор
    ~Stack() {
        clear();
    }

    // Конструктор копирования
    Stack(const Stack& other) : topNode(nullptr), stackSize(0) {
        if (!other.empty()) {
            // Создаем временный стек для сохранения порядка элементов
            Stack<T> temp;
            Node* current = other.topNode;
            while (current != nullptr) {
                temp.push(current->data);
                current = current->next;
            }

            // Переносим элементы из временного стека в текущий
            while (!temp.empty()) {
                push(temp.top());
                temp.pop();
            }
        }
    }

    // Оператор присваивания
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            clear();
            if (!other.empty()) {
                Stack<T> temp;
                Node* current = other.topNode;
                while (current != nullptr) {
                    temp.push(current->data);
                    current = current->next;
                }

                while (!temp.empty()) {
                    push(temp.top());
                    temp.pop();
                }
            }
        }
        return *this;
    }

    // Добавление элемента на вершину стека
    void push(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = topNode;
        topNode = newNode;
        stackSize++;
    }

    // Удаление элемента с вершины стека
    void pop() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }

        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
        stackSize--;
    }

    // Получение элемента с вершины стека
    T& top() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return topNode->data;
    }

    // Получение элемента с вершины стека (константная версия)
    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return topNode->data;
    }

    // Проверка на пустоту
    bool empty() const {
        return topNode == nullptr;
    }

    // Получение размера стека
    size_t size() const {
        return stackSize;
    }

    // Очистка стека
    void clear() {
        while (!empty()) {
            pop();
        }
    }

    // Обмен содержимым с другим стеком
    void swap(Stack& other) {
        std::swap(topNode, other.topNode);
        std::swap(stackSize, other.stackSize);
    }

    // Вывод содержимого стека 
    void print() const {
        if (empty()) {
            std::cout << "Stack is empty" << std::endl;
            return;
        }

        Node* current = topNode;
        std::cout << "Stack (top to bottom): ";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) {
                std::cout << " -> ";
            }
            current = current->next;
        }
        std::cout << std::endl;
    }
};

#endif // STACK_H