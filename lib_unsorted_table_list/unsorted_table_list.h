#ifndef UNSORTED_TABLE_LIST_H
#define UNSORTED_TABLE_LIST_H

#include "itable.h"
#include "list.h"
#include <vector>
#include <utility>
#include <sstream>

template <class TKey, class TVal>
class UnsortedTableOnList : public ITable<TKey, TVal> {
private:
    List<std::pair<TKey, TVal>> _list;

public:
    UnsortedTableOnList() = default;
    ~UnsortedTableOnList() override = default;

    void insert(const TKey& key, const TVal& value) override {
        if (contains(key)) throw TableException("Key already exists");
        _list.push_back({ key, value });
    }

    void remove(const TKey& key) override {
        // Оптимизация: избегаем _list.erase(idx), который делает повторный проход по списку
        for (auto it = _list.begin(); it != _list.end(); ++it) {
            if (it->first == key) {
                *it = _list.back(); // Копируем данные из хвоста в текущий узел
                _list.pop_back();   // Удаляем хвост за O(1)
                return;
            }
        }
        throw TableException("Key not found");
    }

    TVal find(const TKey& key) const override {
        for (auto it = _list.begin(); it != _list.end(); ++it) {
            if (it->first == key) return it->second;
        }
        throw TableException("Key not found");
    }

    bool contains(const TKey& key) const override {
        for (auto it = _list.begin(); it != _list.end(); ++it) {
            if (it->first == key) return true;
        }
        return false;
    }

    void clear() override { _list.clear(); }
    bool isEmpty() const override { return _list.empty(); }

    int size() const override {
        return static_cast<int>(_list.size()); // Приведение типа для избежания warning
    }

    std::vector<TKey> getKeys() const override {
        std::vector<TKey> keys;
        keys.reserve(_list.size());
        for (auto it = _list.begin(); it != _list.end(); ++it) keys.push_back(it->first);
        return keys;
    }

    std::vector<TVal> getValues() const override {
        std::vector<TVal> values;
        values.reserve(_list.size());
        for (auto it = _list.begin(); it != _list.end(); ++it) values.push_back(it->second);
        return values;
    }

    TVal& operator[](const TKey& key) override {
        for (auto it = _list.begin(); it != _list.end(); ++it) {
            if (it->first == key) return it->second;
        }
        _list.push_back({ key, TVal() });
        return _list.back().second; // list.back() отрабатывает за O(1) благодаря tail-указателю
    }

    const TVal& operator[](const TKey& key) const override {
        return find(key);
    }

    std::string serialize() const override {
        std::stringstream ss;
        ss << size() << "\n";
        for (auto it = _list.begin(); it != _list.end(); ++it) {
            ss << it->first << "\n" << it->second << "\n";
        }
        return ss.str();
    }

    void deserialize(const std::string& data) override {
        clear();
        std::stringstream ss(data);
        std::string line;

        if (!std::getline(ss, line) || line.empty()) return;

        int count;
        try {
            count = std::stoi(line);
        }
        catch (...) {
            return;
        }

        for (int i = 0; i < count; ++i) {
            TKey key;
            TVal val;

            // Читаем строку ключа целиком
            if (!std::getline(ss, line)) break;
            std::stringstream keyStream(line);
            keyStream >> key;

            // Читаем строку значения целиком
            if (!std::getline(ss, line)) break;
            std::stringstream valStream(line);
            valStream >> val;

            _list.push_back({ key, val });
        }
    }
};

#endif // UNSORTED_TABLE_LIST_H