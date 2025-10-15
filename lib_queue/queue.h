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
    Node* rearNode;
    size_t queueSize;

public:
    // Конструкторы и деструктор
    Queue() : frontNode(nullptr), rearNode(nullptr), queueSize(0) {}

    Queue(const Queue& other) : frontNode(nullptr), rearNode(nullptr), queueSize(0) {
        // Копирование элементов из другой очереди
        Node* current = other.frontNode;
        while (current != nullptr) {
            enqueue(current->data);
            current = current->next;
        }
    }

    Queue& operator=(const Queue& other) {
        if (this != &other) {
            clear(); // Очищаем текущую очередь

            // Копируем элементы из другой очереди
            Node* current = other.frontNode;
            while (current != nullptr) {
                enqueue(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    ~Queue() {
        clear();
    }

    // Основные операции
    void enqueue(const T& value) {
        Node* newNode = new Node(value);

        if (empty()) {
            frontNode = rearNode = newNode;
        }
        else {
            rearNode->next = newNode;
            rearNode = newNode;
        }

        queueSize++;
    }

    void dequeue() {
        if (empty()) {
            throw std::runtime_error("Cannot dequeue from empty queue");
        }

        Node* temp = frontNode;
        frontNode = frontNode->next;

        // Если очередь стала пустой, обнуляем rearNode
        if (frontNode == nullptr) {
            rearNode = nullptr;
        }

        delete temp;
        queueSize--;
    }

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

    bool empty() const {
        return frontNode == nullptr;
    }

    size_t size() const {
        return queueSize;
    }

    // Дополнительные методы
    void clear() {
        while (!empty()) {
            dequeue();
        }
    }

    void display() const {
        if (empty()) {
            std::cout << "Queue is empty" << std::endl;
            return;
        }

        Node* current = frontNode;
        std::cout << "Queue (front to rear): ";
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

#endif // QUEUE_H