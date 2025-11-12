#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <stdexcept>

template<typename T>
class List {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;

        Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}

        // Явно запрещаем копирование и присваивание узлов
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
    };

    Node* head;
    Node* tail;
    size_t list_size;

    // Вспомогательная функция для копирования списка
    void copyFrom(const List& other) {
        Node* current = other.head;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }

    // Вспомогательная функция для очистки списка
    void clearList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        list_size = 0;
    }

public:
    // Конструкторы и деструктор
    List() : head(nullptr), tail(nullptr), list_size(0) {}

    List(const List& other) : head(nullptr), tail(nullptr), list_size(0) {
        copyFrom(other);
    }

    List(List&& other) noexcept
        : head(other.head), tail(other.tail), list_size(other.list_size) {
        other.head = nullptr;
        other.tail = nullptr;
        other.list_size = 0;
    }

    ~List() {
        clearList();
    }

    // Операторы присваивания
    List& operator=(const List& other) {
        if (this != &other) {
            clearList();
            copyFrom(other);
        }
        return *this;
    }

    List& operator=(List&& other) noexcept {
        if (this != &other) {
            clearList();
            head = other.head;
            tail = other.tail;
            list_size = other.list_size;

            other.head = nullptr;
            other.tail = nullptr;
            other.list_size = 0;
        }
        return *this;
    }

    // Доступ к элементам
    T& front() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        return head->data;
    }

    const T& front() const {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        return head->data;
    }

    T& back() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        return tail->data;
    }

    const T& back() const {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        return tail->data;
    }

    // Итераторы
    class Iterator {
    private:
        Node* current;

    public:
        Iterator(Node* node) : current(node) {}

        T& operator*() {
            if (current == nullptr) {
                throw std::runtime_error("Dereferencing null iterator");
            }
            return current->data;
        }

        Iterator& operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    // Вставка элементов
    void push_front(const T& value) {
        Node* new_node = new Node(value);

        if (empty()) {
            head = tail = new_node;
        }
        else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        list_size++;
    }

    void push_back(const T& value) {
        Node* new_node = new Node(value);

        if (empty()) {
            head = tail = new_node;
        }
        else {
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
        }
        list_size++;
    }

    void insert(size_t position, const T& value) {
        if (position > list_size) {
            throw std::out_of_range("Position out of range");
        }

        if (position == 0) {
            push_front(value);
            return;
        }

        if (position == list_size) {
            push_back(value);
            return;
        }

        Node* new_node = new Node(value);
        Node* current = head;

        // Находим узел на нужной позиции
        for (size_t i = 0; i < position; i++) {
            current = current->next;
        }

        // Вставляем перед current
        new_node->prev = current->prev;
        new_node->next = current;
        current->prev->next = new_node;
        current->prev = new_node;

        list_size++;
    }

    // Удаление элементов
    void pop_front() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }

        Node* temp = head;

        if (head == tail) { // только один элемент
            head = tail = nullptr;
        }
        else {
            head = head->next;
            head->prev = nullptr;
        }

        delete temp;
        list_size--;
    }

    void pop_back() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }

        Node* temp = tail;

        if (head == tail) { // только один элемент
            head = tail = nullptr;
        }
        else {
            tail = tail->prev;
            tail->next = nullptr;
        }

        delete temp;
        list_size--;
    }

    void erase(size_t position) {
        if (position >= list_size) {
            throw std::out_of_range("Position out of range");
        }

        if (position == 0) {
            pop_front();
            return;
        }

        if (position == list_size - 1) {
            pop_back();
            return;
        }

        Node* current = head;
        for (size_t i = 0; i < position; i++) {
            current = current->next;
        }

        // Удаляем current из середины списка
        current->prev->next = current->next;
        current->next->prev = current->prev;

        delete current;
        list_size--;
    }

    void clear() {
        clearList();
    }

    // Информация о списке
    size_t size() const {
        return list_size;
    }

    bool empty() const {
        return list_size == 0;
    }

    // Поиск
    bool contains(const T& value) const {
        return find(value) != -1;
    }

    int find(const T& value) const {
        Node* current = head;
        int index = 0;

        while (current != nullptr) {
            if (current->data == value) {
                return index;
            }
            current = current->next;
            index++;
        }

        return -1;
    }

    // Вывод (для отладки)
    void print() const {
        Node* current = head;
        std::cout << "[";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) {
                std::cout << ", ";
            }
            current = current->next;
        }
        std::cout << "]" << std::endl;
    }
};

#endif // LIST_H