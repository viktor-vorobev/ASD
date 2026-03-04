#ifndef TVECTOR_H
#define TVECTOR_H

#include <stdexcept> // для исключений
#include <initializer_list> // для списка инициализации
#include <algorithm> // для std::copy
#include <iostream>

template<typename T>
class TVector {
private:
    T* _data;
    int _size;
    int _capacity;

    void resize(int new_capacity) {
        if (new_capacity < 0) {
            throw std::invalid_argument("Capacity cannot be negative");
        }

        if (new_capacity == 0) {
            delete[] _data;
            _data = nullptr;
            _size = 0;
            _capacity = 0;
            return;
        }

        T* new_data = new T[new_capacity];

        // Копируем существующие элементы
        int elements_to_copy = (_size < new_capacity) ? _size : new_capacity;
        for (int i = 0; i < elements_to_copy; ++i) {
            new_data[i] = _data[i];
        }

        delete[] _data;
        _data = new_data;
        _capacity = new_capacity;

        if (_size > new_capacity) {
            _size = new_capacity;
        }
    }

public:
    // Конструкторы
    TVector() : _data(nullptr), _size(0), _capacity(0) {}

    explicit TVector(int size) : _size(size), _capacity(size) {
        if (size > 0) {
            _data = new T[size](); // Инициализация нулями
        }
        else if (size == 0) {
            _data = nullptr;
        }
        else {
            throw std::invalid_argument("Size cannot be negative");
        }
    }

    TVector(T* data, int size) : _size(size), _capacity(size) {
        if (size > 0) {
            _data = new T[size];
            std::copy(data, data + size, _data);
        }
        else if (size == 0) {
            _data = nullptr;
        }
        else {
            throw std::invalid_argument("Size cannot be negative");
        }
    }

    TVector(const TVector& other)
        : _size(other._size), _capacity(other._capacity) {
        if (other._size > 0) {
            _data = new T[other._size];
            std::copy(other._data, other._data + other._size, _data);
        }
        else {
            _data = nullptr;
        }
    }

    TVector(std::initializer_list<T> init_list)
        : _size(static_cast<int>(init_list.size())), _capacity(static_cast<int>(init_list.size())) {
        if (_size > 0) {
            _data = new T[_size];
            int i = 0;
            for (const auto& item : init_list) {
                _data[i++] = item;
            }
        }
        else {
            _data = nullptr;
        }
    }

    // Деструктор
    ~TVector() {
        delete[] _data;
    }

    // Оператор присваивания
    TVector& operator=(const TVector& other) {
        if (this != &other) {
            delete[] _data;

            _size = other._size;
            _capacity = other._capacity;

            if (other._size > 0) {
                _data = new T[other._size];
                std::copy(other._data, other._data + other._size, _data);
            }
            else {
                _data = nullptr;
            }
        }
        return *this;
    }

    // Get методы
    int getSize() const { return _size; }
    int getCapacity() const { return _capacity; }
    T* getData() const { return _data; }

    // Основные операции
    void push_front(T val) {
        if (_size == _capacity) {
            resize(_capacity == 0 ? 1 : _capacity * 2);
        }

        // Сдвигаем все элементы вправо
        for (int i = _size; i > 0; --i) {
            _data[i] = _data[i - 1];
        }

        _data[0] = val;
        ++_size;
    }

    void push_back(T val) {
        if (_size == _capacity) {
            resize(_capacity == 0 ? 1 : _capacity * 2);
        }

        _data[_size] = val;
        ++_size;
    }

    void insert(int pos, T val) {
        if (pos < 0 || pos > _size) {
            throw std::out_of_range("Position out of range");
        }

        if (_size == _capacity) {
            resize(_capacity == 0 ? 1 : _capacity * 2);
        }

        // Сдвигаем элементы от pos до конца
        for (int i = _size; i > pos; --i) {
            _data[i] = _data[i - 1];
        }

        _data[pos] = val;
        ++_size;
    }

    void insert(T* pos, T val) {
        if (pos < _data || pos > _data + _size) {
            throw std::out_of_range("Pointer out of range");
        }

        int index = static_cast<int>(pos - _data);
        insert(index, val);
    }

    T pop_front() {
        if (_size == 0) {
            throw std::out_of_range("Vector is empty");
        }

        T value = _data[0];

        // Сдвигаем все элементы влево
        for (int i = 0; i < _size - 1; ++i) {
            _data[i] = _data[i + 1];
        }

        --_size;
        return value;
    }

    T pop_back() {
        if (_size == 0) {
            throw std::out_of_range("Vector is empty");
        }

        --_size;
        return _data[_size];
    }

    T erase(int pos) {
        if (pos < 0 || pos >= _size) {
            throw std::out_of_range("Position out of range");
        }

        T value = _data[pos];

        // Сдвигаем элементы после pos влево
        for (int i = pos; i < _size - 1; ++i) {
            _data[i] = _data[i + 1];
        }

        --_size;
        return value;
    }

    T erase(T* pos) {
        if (pos < _data || pos >= _data + _size) {
            throw std::out_of_range("Pointer out of range");
        }

        int index = static_cast<int>(pos - _data);
        return erase(index);
    }

    T front() {
        if (_size == 0) {
            throw std::out_of_range("Vector is empty");
        }
        return _data[0];
    }

    T back() {
        if (_size == 0) {
            throw std::out_of_range("Vector is empty");
        }
        return _data[_size - 1];
    }

    int find(T val) {
        for (int i = 0; i < _size; ++i) {
            if (_data[i] == val) {
                return i;
            }
        }
        return -1; // Возвращаем -1, если не найдено
    }

    void repacking(int pos) {
        if (pos < 0 || pos >= _size) return;

        // Сдвигаем элементы от pos до конца в начало
        int new_size = _size - pos;
        for (int i = 0; i < new_size; ++i) {
            _data[i] = _data[pos + i];
        }
        _size = new_size;
    }

    bool is_empty() const { return _size == 0; }
    bool is_full() const { return _size == _capacity; }
    T* data() { return _data; }
    const T* data() const { return _data; } 
    int size() const { return _size; }

    // Операторы доступа
    T& operator[](int index) {
        if (index < 0 || index >= _size) {
            throw std::out_of_range("Index out of range");
        }
        return _data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= _size) {
            throw std::out_of_range("Index out of range");
        }
        return _data[index];
    }

    // Дополнительные полезные методы
    void clear() { _size = 0; }

    void shrink_to_fit() {
        if (_capacity > _size) {
            resize(_size);
        }
    }

    void reserve(int new_capacity) {
        if (new_capacity < 0) {
            throw std::invalid_argument("Capacity cannot be negative");
        }

        if (new_capacity > _capacity) {
            resize(new_capacity);
        }
    }
};

#endif // TVECTOR_H