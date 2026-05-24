#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "../lib_itable/itable.cpp"
#include "../lib_list/list.h"
#include <vector>
#include <functional>
#include <sstream>
#include <string>
#include <utility>

template <class TVal>
struct HashData {
    std::string key;
    TVal value;

    HashData() = default;
    HashData(std::string k, TVal v) : key(std::move(k)), value(std::move(v)) {}
};

template <class TVal>
class HashTable : public ITable<std::string, TVal> {
private:
    std::vector<List<HashData<TVal>>> _table;
    int _size = 0;

    size_t hashFunc(const std::string& key) const {
        return std::hash<std::string>()(key) % _table.size();
    }

    void rehash() {
        std::vector<List<HashData<TVal>>> old = std::move(_table);
        _table.resize(old.size() * 2 + 1);
        _size = 0;
        for (auto& list : old) {
            for (auto it = list.begin(); it != list.end(); ++it) {
                insert(std::move(it->key), std::move(it->value));
            }
        }
    }

public:
    HashTable(size_t initial_capacity = 11) { _table.resize(initial_capacity); }
    ~HashTable() override = default;

    void insert(const std::string& key, const TVal& value) override {
        if (_size >= _table.size() * 0.75) rehash();
        size_t idx = hashFunc(key);
        for (auto it = _table[idx].begin(); it != _table[idx].end(); ++it) {
            if (it->key == key) throw TableException("Key already exists");
        }
        _table[idx].push_back(HashData<TVal>(key, value));
        _size++;
    }

    void remove(const std::string& key) override {
        size_t idx = hashFunc(key);
        for (auto it = _table[idx].begin(); it != _table[idx].end(); ++it) {
            if (it->key == key) {
                *it = std::move(_table[idx].back());
                _table[idx].pop_back();
                _size--;
                return;
            }
        }
        throw TableException("Key not found");
    }

    TVal find(const std::string& key) const override {
        size_t idx = hashFunc(key);
        for (auto it = _table[idx].begin(); it != _table[idx].end(); ++it) {
            if (it->key == key) return it->value;
        }
        throw TableException("Key not found");
    }

    bool contains(const std::string& key) const override {
        size_t idx = hashFunc(key);
        for (auto it = _table[idx].begin(); it != _table[idx].end(); ++it) {
            if (it->key == key) return true;
        }
        return false;
    }

    void clear() override {
        for (auto& list : _table) list.clear();
        _size = 0;
    }

    bool isEmpty() const override { return _size == 0; }
    int size() const override { return _size; }

    std::vector<std::string> getKeys() const override {
        std::vector<std::string> keys;
        keys.reserve(_size);
        for (const auto& list : _table) {
            for (auto it = list.begin(); it != list.end(); ++it) keys.push_back(it->key);
        }
        return keys;
    }

    std::vector<TVal> getValues() const override {
        std::vector<TVal> vals;
        vals.reserve(_size);
        for (const auto& list : _table) {
            for (auto it = list.begin(); it != list.end(); ++it) vals.push_back(it->value);
        }
        return vals;
    }

    TVal& operator[](const std::string& key) override {
        size_t idx = hashFunc(key);
        for (auto it = _table[idx].begin(); it != _table[idx].end(); ++it) {
            if (it->key == key) return it->value;
        }

        insert(key, TVal());
        idx = hashFunc(key);
        return _table[idx].back().value;
    }

    const TVal& operator[](const std::string& key) const override { return find(key); }

    std::string serialize() const override {
        std::stringstream ss;
        ss << size() << "\n";
        for (const auto& list : _table) {
            for (auto it = list.begin(); it != list.end(); ++it) {
                ss << it->key << "\n" << it->value << "\n";
            }
        }
        return ss.str();
    }

    void deserialize(const std::string& data) override {
        clear();
        std::stringstream ss(data);
        std::string line;

        if (!std::getline(ss, line) || line.empty()) return;

        int count;
        try { count = std::stoi(line); }
        catch (...) { return; }

        for (int i = 0; i < count; ++i) {
            std::string key; TVal val;
            if (!std::getline(ss, line)) break;
            std::stringstream keyStream(line);
            keyStream >> key;

            if (!std::getline(ss, line)) break;
            std::stringstream valStream(line);
            valStream >> val;

            insert(key, val);
        }
    }
};

template <class T>
HashTable<T>& merge_dict(std::vector<std::pair<std::string, T>> dict1, std::vector<std::pair<std::string, T>> dict2) {
    HashTable<T>* merged_table = new HashTable<T>();

    for (const auto& pair : dict1) {
        merged_table->insert(pair.first, pair.second);
    }

    for (const auto& pair : dict2) {
        if (!merged_table->contains(pair.first)) {
            merged_table->insert(pair.first, pair.second);
        }
    }

    return *merged_table;
}

#endif // HASH_TABLE_H