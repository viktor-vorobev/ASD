#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"
#include <stdexcept>

template<typename T>
class Queue {
private:
    List<T> list;

public:
    Queue() = default;

    // Конструктор копирования
    Queue(const Queue& other) : list(other.list) {}

    // Конструктор перемещения
    Queue(Queue&& other) noexcept : list(std::move(other.list)) {}

    // Оператор присваивания
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            list = other.list;
        }
        return *this;
    }

    // Оператор перемещающего присваивания
    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            list = std::move(other.list);
        }
        return *this;
    }

    // Добавление элемента в очередь
    void enqueue(const T& value) {
        list.push_back(value);
    }

    // Удаление и возврат первого элемента
    T dequeue() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        T value = list.front();
        list.pop_front();
        return value;
    }

    // Просмотр первого элемента без удаления
    T& front() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return list.front();
    }

    const T& front() const {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return list.front();
    }

    // Просмотр последнего элемента
    T& back() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return list.back();
    }

    const T& back() const {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return list.back();
    }

    // Проверка на пустоту
    bool empty() const {
        return list.empty();
    }

    // Размер очереди
    size_t size() const {
        return list.size();
    }

    // Очистка очереди
    void clear() {
        list.clear();
    }
};

#endif // QUEUE_H