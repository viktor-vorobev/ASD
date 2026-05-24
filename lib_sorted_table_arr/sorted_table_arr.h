#ifndef SORTED_TABLE_ARR_H
#define SORTED_TABLE_ARR_H

#include "itable.h"
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>

template <class TKey, class TVal>
class SortedTableOnArr : public ITable<TKey, TVal> {
private:
    std::vector<std::pair<TKey, TVal>> _rows;

    auto findIt(const TKey& key) {
        return std::lower_bound(_rows.begin(), _rows.end(), key,
            [](const std::pair<TKey, TVal>& pair, const TKey& k) { return pair.first < k; });
    }

    auto findIt(const TKey& key) const {
        return std::lower_bound(_rows.begin(), _rows.end(), key,
            [](const std::pair<TKey, TVal>& pair, const TKey& k) { return pair.first < k; });
    }

public:
    SortedTableOnArr() = default;
    ~SortedTableOnArr() override = default;

    void insert(const TKey& key, const TVal& value) override {
        auto it = findIt(key);
        if (it != _rows.end() && it->first == key) throw TableException("Key already exists");
        // Используем emplace для прямого конструирования элемента
        _rows.emplace(it, key, value);
    }

    void remove(const TKey& key) override {
        auto it = findIt(key);
        if (it == _rows.end() || it->first != key) throw TableException("Key not found");
        _rows.erase(it); // В векторе это O(N), так как нужно сдвинуть элементы
    }

    TVal find(const TKey& key) const override {
        auto it = findIt(key);
        if (it != _rows.end() && it->first == key) return it->second;
        throw TableException("Key not found");
    }

    bool contains(const TKey& key) const override {
        auto it = findIt(key);
        return (it != _rows.end() && it->first == key);
    }

    void clear() override { _rows.clear(); }
    bool isEmpty() const override { return _rows.empty(); }
    int size() const override { return static_cast<int>(_rows.size()); }

    std::vector<TKey> getKeys() const override {
        std::vector<TKey> keys;
        keys.reserve(_rows.size());
        for (const auto& row : _rows) keys.push_back(row.first);
        return keys;
    }

    std::vector<TVal> getValues() const override {
        std::vector<TVal> values;
        values.reserve(_rows.size());
        for (const auto& row : _rows) values.push_back(row.second);
        return values;
    }

    TVal& operator[](const TKey& key) override {
        auto it = findIt(key);
        if (it != _rows.end() && it->first == key) return it->second;
        // emplace возвращает итератор на вставленный элемент (начиная с C++11)
        it = _rows.emplace(it, key, TVal());
        return it->second;
    }

    const TVal& operator[](const TKey& key) const override {
        auto it = findIt(key);
        if (it != _rows.end() && it->first == key) return it->second;
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
            return;
        }

        for (int i = 0; i < count; ++i) {
            TKey key;
            TVal val;

            if (!std::getline(ss, line)) break;
            std::stringstream keyStream(line);
            keyStream >> key;

            if (!std::getline(ss, line)) break;
            std::stringstream valStream(line);
            valStream >> val;

            _rows.emplace_back(key, val); // Вставляем в конец
        }
        // Сортируем все разом за O(N log N)
        std::sort(_rows.begin(), _rows.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    }
};

#endif // SORTED_TABLE_ARR_H