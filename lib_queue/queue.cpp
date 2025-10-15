#include "queue.h"
#include <iostream>
#include <stdexcept>

// Реализация методов шаблонного класса Queue


template<typename T>
Queue<T>::Queue() : frontNode(nullptr), rearNode(nullptr), queueSize(0) {}


template<typename T>
Queue<T>::Queue(const Queue& other) : frontNode(nullptr), rearNode(nullptr), queueSize(0) {
    // Копирование элементов из другой очереди
    Node* current = other.frontNode;
    while (current != nullptr) {
        enqueue(current->data);
        current = current->next;
    }
}


template<typename T>
Queue<T>& Queue<T>::operator=(const Queue& other) {
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


template<typename T>
Queue<T>::~Queue() {
    clear();
}


template<typename T>
void Queue<T>::enqueue(const T& value) {
    Node* newNode = new Node(value);

    if (empty()) {
        // Если очередь пуста, новый узел становится и началом и концом
        frontNode = rearNode = newNode;
    }
    else {
        // Добавляем новый узел в конец и обновляем rearNode
        rearNode->next = newNode;
        rearNode = newNode;
    }

    queueSize++;
}


template<typename T>
void Queue<T>::dequeue() {
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


template<typename T>
T& Queue<T>::front() {
    if (empty()) {
        throw std::runtime_error("Queue is empty");
    }
    return frontNode->data;
}


template<typename T>
const T& Queue<T>::front() const {
    if (empty()) {
        throw std::runtime_error("Queue is empty");
    }
    return frontNode->data;
}


template<typename T>
bool Queue<T>::empty() const {
    return frontNode == nullptr;
}


template<typename T>
size_t Queue<T>::size() const {
    return queueSize;
}


template<typename T>
void Queue<T>::clear() {
    while (!empty()) {
        dequeue();
    }
}


template<typename T>
void Queue<T>::display() const {
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

// Явная инстанциация шаблонов для компиляции
template class Queue<int>;
template class Queue<double>;
template class Queue<std::string>;