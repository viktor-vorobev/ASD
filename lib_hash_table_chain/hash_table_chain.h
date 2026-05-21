#ifndef HASH_TABLE_CHAIN_H
#define HASH_TABLE_CHAIN_H

#include "itable.h"
#include "list.h"
#include <vector>
#include <functional>

template <class TKey, class TVal>
class HashTableChaining : public ITable<TKey, TVal> {
private:
    std::vector<List<std::pair<TKey, TVal>>> _table;
    int _size = 0;

    size_t hashFunc(const TKey& key) const {
        return std::hash<TKey>()(key) % _table.size();
    }

    void rehash() {
        std::vector<List<std::pair<TKey, TVal>>> old = _table;
        _table.clear();
        _table.resize(old.size() * 2 + 1);
        _size = 0;
        for (auto& list : old) {
            for (auto it = list.begin(); it != list.end(); ++it) {
                insert(it->first, it->second);
            }
        }
    }

public:
    HashTableChaining(size_t initial_capacity = 11) { _table.resize(initial_capacity); }
    ~HashTableChaining() override = default;

    void insert(const TKey& key, const TVal& value) override {
        if (_size >= _table.size() * 0.75) rehash();
        size_t idx = hashFunc(key);
        for (auto it = _table[idx].begin(); it != _table[idx].end(); ++it) {
            if (it->first == key) throw TableException("Key already exists");
        }
        _table[idx].push_back({ key, value });
        _size++;
    }

    void remove(const TKey& key) override {
        size_t idx = hashFunc(key);
        size_t pos = 0;
        for (auto it = _table[idx].begin(); it != _table[idx].end(); ++it, ++pos) {
            if (it->first == key) {
                _table[idx].erase(pos);
                _size--;
                return;
            }
        }
        throw TableException("Key not found");
    }

    TVal find(const TKey& key) const override {
        size_t idx = hashFunc(key);
        for (auto it = _table[idx].begin(); it != _table[idx].end(); ++it) {
            if (it->first == key) return it->second;
        }
        throw TableException("Key not found");
    }

    bool contains(const TKey& key) const override {
        size_t idx = hashFunc(key);
        for (auto it = _table[idx].begin(); it != _table[idx].end(); ++it) {
            if (it->first == key) return true;
        }
        return false;
    }

    void clear() override {
        for (auto& list : _table) list.clear();
        _size = 0;
    }
    bool isEmpty() const override { return _size == 0; }
    int size() const override { return _size; }

    std::vector<TKey> getKeys() const override {
        std::vector<TKey> keys;
        for (const auto& list : _table) {
            for (auto it = list.begin(); it != list.end(); ++it) keys.push_back(it->first);
        }
        return keys;
    }

    std::vector<TVal> getValues() const override {
        std::vector<TVal> vals;
        for (const auto& list : _table) {
            for (auto it = list.begin(); it != list.end(); ++it) vals.push_back(it->second);
        }
        return vals;
    }

    TVal& operator[](const TKey& key) override {
        size_t idx = hashFunc(key);
        for (auto it = _table[idx].begin(); it != _table[idx].end(); ++it) {
            if (it->first == key) return it->second;
        }
        insert(key, TVal());
        for (auto it = _table[idx].begin(); it != _table[idx].end(); ++it) {
            if (it->first == key) return it->second;
        }
        throw TableException("Insert failed");
    }

    const TVal& operator[](const TKey& key) const override { return find(key); }

    std::string serialize() const override { return ""; } // Заглушка для сокращения кода
    void deserialize(const std::string& data) override {}
};
#endif