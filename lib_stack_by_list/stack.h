#ifndef STACK_H
#define STACK_H

#include "list.h"
#include <stdexcept>

template<typename T>
class Stack {
private:
    List<T> list;

public:
    Stack() = default;

    // Конструктор копирования
    Stack(const Stack& other) : list(other.list) {}

    // Конструктор перемещения
    Stack(Stack&& other) noexcept : list(std::move(other.list)) {}

    // Оператор присваивания
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            list = other.list;
        }
        return *this;
    }

    // Оператор перемещающего присваивания
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            list = std::move(other.list);
        }
        return *this;
    }

    // Добавление элемента в стек
    void push(const T& value) {
        list.push_front(value);
    }

    // Удаление и возврат верхнего элемента
    T pop() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        T value = list.front();
        list.pop_front();
        return value;
    }

    // Просмотр верхнего элемента без удаления
    T& top() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return list.front();
    }

    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return list.front();
    }

    // Проверка на пустоту
    bool empty() const {
        return list.empty();
    }

    // Размер стека
    size_t size() const {
        return list.size();
    }

    // Очистка стека
    void clear() {
        list.clear();
    }
};

#endif // STACK_H