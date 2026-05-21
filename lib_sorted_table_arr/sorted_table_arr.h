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
        _rows.insert(it, { key, value });
    }

    void remove(const TKey& key) override {
        auto it = findIt(key);
        if (it == _rows.end() || it->first != key) throw TableException("Key not found");
        _rows.erase(it);
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
        for (const auto& row : _rows) keys.push_back(row.first);
        return keys;
    }

    std::vector<TVal> getValues() const override {
        std::vector<TVal> values;
        for (const auto& row : _rows) values.push_back(row.second);
        return values;
    }

    TVal& operator[](const TKey& key) override {
        auto it = findIt(key);
        if (it != _rows.end() && it->first == key) return it->second;
        it = _rows.insert(it, { key, TVal() });
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
        int count = std::stoi(line);
        for (int i = 0; i < count; ++i) {
            TKey key;
            if (!std::getline(ss, line)) break;
            std::stringstream(line) >> key;
            TVal val;
            ss >> val;
            _rows.push_back({ key, val });
        }
        std::sort(_rows.begin(), _rows.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    }
};

#endif // SORTED_TABLE_ARR_H