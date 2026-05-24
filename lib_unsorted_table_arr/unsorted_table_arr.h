#ifndef UNSORTED_TABLE_ARR_H
#define UNSORTED_TABLE_ARR_H

#include "itable.h"
#include <vector>
#include <utility>
#include <sstream>

template <class TKey, class TVal>
class UnsortedTableOnArr : public ITable<TKey, TVal> {
private:
    std::vector<std::pair<TKey, TVal>> _rows;
public:
    UnsortedTableOnArr() = default;
    ~UnsortedTableOnArr() override = default;

    void insert(const TKey& key, const TVal& value) override {
        if (contains(key)) throw TableException("Key already exists");
        // Используем emplace_back для избежания лишних копирований pair
        _rows.emplace_back(key, value);
    }

    void remove(const TKey& key) override {
        for (auto it = _rows.begin(); it != _rows.end(); ++it) {
            if (it->first == key) {
                *it = _rows.back();
                _rows.pop_back();
                return;
            }
        }
        throw TableException("Key not found");
    }

    TVal find(const TKey& key) const override {
        for (const auto& row : _rows) {
            if (row.first == key) return row.second;
        }
        throw TableException("Key not found");
    }

    bool contains(const TKey& key) const override {
        for (const auto& row : _rows) {
            if (row.first == key) return true;
        }
        return false;
    }

    void clear() override { _rows.clear(); }
    bool isEmpty() const override { return _rows.empty(); }
    int size() const override { return static_cast<int>(_rows.size()); }

    std::vector<TKey> getKeys() const override {
        std::vector<TKey> keys;
        keys.reserve(_rows.size()); // Оптимизация выделения памяти
        for (const auto& row : _rows) keys.push_back(row.first);
        return keys;
    }

    std::vector<TVal> getValues() const override {
        std::vector<TVal> values;
        values.reserve(_rows.size()); // Оптимизация выделения памяти
        for (const auto& row : _rows) values.push_back(row.second);
        return values;
    }

    TVal& operator[](const TKey& key) override {
        for (auto& row : _rows) {
            if (row.first == key) return row.second;
        }
        _rows.emplace_back(key, TVal());
        return _rows.back().second;
    }

    const TVal& operator[](const TKey& key) const override {
        for (const auto& row : _rows) {
            if (row.first == key) return row.second;
        }
        throw TableException("Key not found");
    }

    std::string serialize() const override {
        std::stringstream ss;
        ss << _rows.size() << "\n";
        for (const auto& row : _rows) {
            ss << row.first << "\n" << row.second << "\n";
        }
        return ss.str();
    }

    void deserialize(const std::string& data) override {
        _rows.clear();
        std::stringstream ss(data);
        std::string line;

        if (!std::getline(ss, line) || line.empty()) return;

        int count;
        try {
            count = std::stoi(line);
        }
        catch (...) {
            return; // Защита от поврежденных данных
        }

        for (int i = 0; i < count; ++i) {
            TKey key;
            TVal val;

            // 1. Читаем строку ключа целиком
            if (!std::getline(ss, line)) break;
            std::stringstream keyStream(line);
            keyStream >> key; // Чтение для шаблонного типа

            // 2. Читаем строку значения целиком в буфер
            if (!std::getline(ss, line)) break;
            std::stringstream valStream(line);
            valStream >> val; // Парсинг изолированной строки

            _rows.emplace_back(key, val);
        }
    }
};

#endif // UNSORTED_TABLE_ARR_H