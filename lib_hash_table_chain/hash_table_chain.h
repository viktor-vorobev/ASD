#ifndef HASH_TABLE_CHAIN_H
#define HASH_TABLE_CHAIN_H

#include "itable.h"
#include "list.h"
#include <vector>
#include <functional>
#include <sstream>
#include <string>

template <class TKey, class TVal>
class HashTableChaining : public ITable<TKey, TVal> {
private:
    std::vector<List<std::pair<TKey, TVal>>> _table;
    int _size = 0;

    size_t hashFunc(const TKey& key) const {
        return std::hash<TKey>()(key) % _table.size();
    }

    void rehash() {
        std::vector<List<std::pair<TKey, TVal>>> old = std::move(_table);
        _table.resize(old.size() * 2 + 1);
        _size = 0; // Будет инкрементироваться внутри insert
        for (auto& list : old) {
            for (auto it = list.begin(); it != list.end(); ++it) {
                insert(std::move(it->first), std::move(it->second));
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
        for (auto it = _table[idx].begin(); it != _table[idx].end(); ++it) {
            if (it->first == key) {
                *it = std::move(_table[idx].back());
                _table[idx].pop_back();
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
        keys.reserve(_size);
        for (const auto& list : _table) {
            for (auto it = list.begin(); it != list.end(); ++it) keys.push_back(it->first);
        }
        return keys;
    }

    std::vector<TVal> getValues() const override {
        std::vector<TVal> vals;
        vals.reserve(_size);
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

        idx = hashFunc(key);

        // После вставки элемент находится в конце списка этой корзины
        return _table[idx].back().second;
    }

    const TVal& operator[](const TKey& key) const override { return find(key); }

    std::string serialize() const override {
        std::stringstream ss;
        ss << size() << "\n";
        for (const auto& list : _table) {
            for (auto it = list.begin(); it != list.end(); ++it) {
                ss << it->first << "\n" << it->second << "\n";
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
            TKey key; TVal val;
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
#endif // HASH_TABLE_CHAIN_H