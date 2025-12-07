#ifndef CYCLE_LIST_H
#define CYCLE_LIST_H

#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <functional>

template<typename T>
class CycleList {
private:
    struct CycleNode {
        T value;
        CycleNode* next;
        CycleNode* prev;

        CycleNode(const T& val) : value(val), next(nullptr), prev(nullptr) {}
    };

    CycleNode* head;
    size_t size_;

    // Ключевое исправление: CycleNode* внутри шаблона должен быть определен
    std::unordered_map<T, typename CycleList<T>::CycleNode*> valueToNode;
    // Или просто CycleNode*, так как мы внутри класса CycleList<T>
    // std::unordered_map<T, CycleNode*> valueToNode; // Это тоже должно работать

public:
    CycleList() : head(nullptr), size_(0) {}

    ~CycleList() {
        clear();
    }

    // Добавление элемента в конец циклического списка
    void push_back(const T& value) {
        CycleNode* newNode = new CycleNode(value);

        if (empty()) {
            head = newNode;
            head->next = head;
            head->prev = head;
        }
        else {
            CycleNode* tail = head->prev;

            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
        }

        valueToNode[value] = newNode;
        size_++;
    }

    // Добавление элемента в начало
    void push_front(const T& value) {
        push_back(value);
        head = head->prev; // Сдвигаем head на новый элемент
    }

    // Удаление элемента по значению
    bool remove(const T& value) {
        auto it = valueToNode.find(value);
        if (it == valueToNode.end()) {
            return false; // Элемент не найден
        }

        CycleNode* nodeToRemove = it->second;

        if (size_ == 1) {
            head = nullptr;
        }
        else {
            nodeToRemove->prev->next = nodeToRemove->next;
            nodeToRemove->next->prev = nodeToRemove->prev;

            if (nodeToRemove == head) {
                head = head->next;
            }
        }

        delete nodeToRemove;
        valueToNode.erase(it);
        size_--;

        return true;
    }

    // Поиск элемента
    bool contains(const T& value) const {
        return valueToNode.find(value) != valueToNode.end();
    }

    // Получение следующего элемента после заданного
    T get_next(const T& value) const {
        auto it = valueToNode.find(value);
        if (it == valueToNode.end()) {
            throw std::runtime_error("Element not found");
        }

        return it->second->next->value;
    }

    // Получение предыдущего элемента перед заданным
    T get_prev(const T& value) const {
        auto it = valueToNode.find(value);
        if (it == valueToNode.end()) {
            throw std::runtime_error("Element not found");
        }

        return it->second->prev->value;
    }

    // Ротация списка (сдвиг головы)
    void rotate_forward() {
        if (!empty()) {
            head = head->next;
        }
    }

    void rotate_backward() {
        if (!empty()) {
            head = head->prev;
        }
    }

    // Получение всех элементов по порядку
    std::vector<T> get_elements() const {
        std::vector<T> result;
        if (empty()) return result;

        CycleNode* current = head;
        do {
            result.push_back(current->value);
            current = current->next;
        } while (current != head);

        return result;
    }

    // Получение элементов в обратном порядке
    std::vector<T> get_elements_reverse() const {
        std::vector<T> result;
        if (empty()) return result;

        CycleNode* current = head->prev;
        do {
            result.push_back(current->value);
            current = current->prev;
        } while (current != head->prev);

        return result;
    }

    // Основные операции
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }

    T front() const {
        if (empty()) {
            throw std::runtime_error("CycleList is empty");
        }
        return head->value;
    }

    T back() const {
        if (empty()) {
            throw std::runtime_error("CycleList is empty");
        }
        return head->prev->value;
    }

    // Очистка списка
    void clear() {
        if (empty()) return;

        CycleNode* current = head;
        CycleNode* nextNode;

        do {
            nextNode = current->next;
            delete current;
            current = nextNode;
        } while (current != head);

        head = nullptr;
        size_ = 0;
        valueToNode.clear();
    }

    // Обмен содержимым с другим списком
    void swap(CycleList& other) {
        std::swap(head, other.head);
        std::swap(size_, other.size_);
        valueToNode.swap(other.valueToNode);
    }

    // Вывод для отладки
    void print() const {
        if (empty()) {
            std::cout << "CycleList is empty" << std::endl;
            return;
        }

        CycleNode* current = head;
        std::cout << "CycleList: ";
        do {
            std::cout << current->value;
            if (current->next != head) {
                std::cout << " <-> ";
            }
            current = current->next;
        } while (current != head);
        std::cout << std::endl;
    }

    // Итератор для обхода списка
    class Iterator {
    private:
        CycleNode* current;
        CycleNode* start;
        bool firstIteration;

    public:
        Iterator(CycleNode* node) : current(node), start(node), firstIteration(true) {}

        T& operator*() { return current->value; }
        const T& operator*() const { return current->value; }

        Iterator& operator++() {
            current = current->next;
            firstIteration = false;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current && firstIteration == other.firstIteration;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(head); } // Для циклического списка begin == end

    // Константный итератор
    class ConstIterator {
    private:
        const CycleNode* current;
        const CycleNode* start;
        bool firstIteration;

    public:
        ConstIterator(const CycleNode* node) : current(node), start(node), firstIteration(true) {}

        const T& operator*() const { return current->value; }

        ConstIterator& operator++() {
            current = current->next;
            firstIteration = false;
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const ConstIterator& other) const {
            return current == other.current && firstIteration == other.firstIteration;
        }

        bool operator!=(const ConstIterator& other) const {
            return !(*this == other);
        }
    };

    ConstIterator cbegin() const { return ConstIterator(head); }
    ConstIterator cend() const { return ConstIterator(head); }
};

#endif // LIST_CYCLE_H