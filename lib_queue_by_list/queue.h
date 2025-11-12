#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <stdexcept>

template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* frontNode;
    Node* backNode;
    size_t queue_size;

public:
    // Конструктор по умолчанию - ЯВНАЯ ИНИЦИАЛИЗАЦИЯ
    Queue() : frontNode(nullptr), backNode(nullptr), queue_size(0) {
        // Можно добавить отладочный вывод
        // std::cout << "Queue constructor called" << std::endl;
    }

    // Конструктор копирования
    Queue(const Queue& other) : frontNode(nullptr), backNode(nullptr), queue_size(0) {
        Node* current = other.frontNode;
        while (current != nullptr) {
            enqueue(current->data);
            current = current->next;
        }
    }

    // Конструктор перемещения
    Queue(Queue&& other) noexcept
        : frontNode(other.frontNode), backNode(other.backNode), queue_size(other.queue_size) {
        other.frontNode = nullptr;
        other.backNode = nullptr;
        other.queue_size = 0;
    }

    // Деструктор
    ~Queue() {
        clear();
    }

    // Оператор присваивания
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            clear();
            Node* current = other.frontNode;
            while (current != nullptr) {
                enqueue(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    // Оператор перемещающего присваивания
    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            clear();
            frontNode = other.frontNode;
            backNode = other.backNode;
            queue_size = other.queue_size;

            other.frontNode = nullptr;
            other.backNode = nullptr;
            other.queue_size = 0;
        }
        return *this;
    }

    // Добавление элемента в очередь
    void enqueue(const T& value) {
        Node* newNode = new Node(value);

        if (empty()) {
            frontNode = backNode = newNode;
        }
        else {
            backNode->next = newNode;
            backNode = newNode;
        }
        queue_size++;
    }

    // Удаление и возврат первого элемента
    T dequeue() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }

        Node* temp = frontNode;
        T value = temp->data;

        frontNode = frontNode->next;
        if (frontNode == nullptr) {
            backNode = nullptr; // Очередь стала пустой
        }

        delete temp;
        queue_size--;
        return value;
    }

    // Просмотр первого элемента без удаления
    T& front() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return frontNode->data;
    }

    const T& front() const {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return frontNode->data;
    }

    // Просмотр последнего элемента
    T& back() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return backNode->data;
    }

    const T& back() const {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return backNode->data;
    }

    // Проверка на пустоту
    bool empty() const {
        return frontNode == nullptr;
    }

    // Размер очереди
    size_t size() const {
        return queue_size;
    }

    // Очистка очереди
    void clear() {
        while (!empty()) {
            dequeue();
        }
    }

    // Вывод для отладки
    void print() const {
        Node* current = frontNode;
        std::cout << "Queue: ";
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

#endif // QUEUE_H