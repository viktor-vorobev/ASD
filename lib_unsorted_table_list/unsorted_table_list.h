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
        size_t idx = 0;
        for (auto it = _list.begin(); it != _list.end(); ++it, ++idx) {
            if (it->first == key) {
                _list.erase(idx);
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
    int size() const override { return _list.size(); }

    std::vector<TKey> getKeys() const override {
        std::vector<TKey> keys;
        for (auto it = _list.begin(); it != _list.end(); ++it) keys.push_back(it->first);
        return keys;
    }

    std::vector<TVal> getValues() const override {
        std::vector<TVal> values;
        for (auto it = _list.begin(); it != _list.end(); ++it) values.push_back(it->second);
        return values;
    }

    TVal& operator[](const TKey& key) override {
        for (auto it = _list.begin(); it != _list.end(); ++it) {
            if (it->first == key) return it->second;
        }
        _list.push_back({ key, TVal() });
        return _list.back().second;
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
        int count = std::stoi(line);
        for (int i = 0; i < count; ++i) {
            TKey key;
            if (!std::getline(ss, line)) break;
            std::stringstream(line) >> key;
            TVal val;
            ss >> val;
            insert(key, val);
        }
    }
};

#endif // UNSORTED_TABLE_LIST_H