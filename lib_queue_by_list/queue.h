#ifndef QUEUE_LIST_BASED_H
#define QUEUE_LIST_BASED_H

#include "list.h"
#include <stdexcept>

template<typename T>
class Queue {
private:
    List<T> list;  // Используем двусвязный список как основу

public:
    // Конструкторы
    Queue() = default;

    Queue(const Queue& other) : list(other.list) {}

    Queue(Queue&& other) noexcept : list(std::move(other.list)) {}

    // Деструктор (не нужен явно, List сам очистится)
    ~Queue() = default;

    // Операторы присваивания
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            list = other.list;
        }
        return *this;
    }

    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            list = std::move(other.list);
        }
        return *this;
    }

    // Основные операции очереди

    // Добавление элемента в конец (enqueue)
    void enqueue(const T& value) {
        list.push_back(value);
    }

    // Добавление с перемещением (C++11)
    void enqueue(T&& value) {
        list.push_back(std::move(value));
    }

    // Удаление элемента из начала (dequeue)
    void dequeue() {
        if (empty()) {
            throw std::runtime_error("Cannot dequeue from empty queue");
        }
        list.pop_front();
    }

    // Доступ к первому элементу (front)
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

    // Доступ к последнему элементу (back)
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

    // Итераторы
    typename List<T>::Iterator begin() {
        return list.begin();
    }

    typename List<T>::Iterator end() {
        return list.end();
    }

    typename List<T>::ConstIterator begin() const {
        return list.begin();
    }

    typename List<T>::ConstIterator end() const {
        return list.end();
    }

    typename List<T>::ConstIterator cbegin() const {
        return list.cbegin();
    }

    typename List<T>::ConstIterator cend() const {
        return list.cend();
    }

    // Вывод для отладки
    void print() const {
        std::cout << "Queue (front to back): ";
        list.print();
    }

    // Проверка наличия элемента
    bool contains(const T& value) const {
        return list.contains(value);
    }

    // Поиск элемента
    int find(const T& value) const {
        return list.find(value);
    }
};



#endif // QUEUE_LIST_BASED_H