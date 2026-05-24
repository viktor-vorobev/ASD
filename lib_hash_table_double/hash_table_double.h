#ifndef HASH_TABLE_DOUBLE_H
#define HASH_TABLE_DOUBLE_H

#include "itable.h"
#include <vector>
#include <functional>
#include <sstream>
#include <string>

template <class TKey, class TVal>
class HashTableDouble : public ITable<TKey, TVal> {
private:
    enum Status { EMPTY, OCCUPIED, DELETED };
    struct Entry { TKey key; TVal val; Status status = EMPTY; };

    std::vector<Entry> _table;
    int _size = 0;

    size_t hash1(const TKey& key) const { return std::hash<TKey>()(key) % _table.size(); }
    size_t hash2(const TKey& key) const {
        size_t p = 7;
        return p - (std::hash<TKey>()(key) % p);
    }

    void rehash() {
        std::vector<Entry> old = std::move(_table);
        _table.resize(old.size() * 2 + 1);
        for (auto& e : _table) e.status = EMPTY;

        _size = 0;
        for (auto& e : old) {
            if (e.status == OCCUPIED) {
                insert(std::move(e.key), std::move(e.val));
            }
        }
    }

public:
    HashTableDouble(size_t cap = 11) { _table.resize(cap); }
    ~HashTableDouble() override = default;

    void insert(const TKey& key, const TVal& value) override {
        if (_size >= _table.size() * 0.5) rehash();
        size_t idx = hash1(key);
        size_t step = hash2(key);

        for (size_t i = 0; i < _table.size(); ++i) {
            size_t probe = (idx + i * step) % _table.size();
            if (_table[probe].status == OCCUPIED && _table[probe].key == key) throw TableException("Key already exists");
            if (_table[probe].status != OCCUPIED) {
                _table[probe] = { key, value, OCCUPIED };
                _size++;
                return;
            }
        }
    }

    void remove(const TKey& key) override {
        size_t idx = hash1(key);
        size_t step = hash2(key);
        for (size_t i = 0; i < _table.size(); ++i) {
            size_t probe = (idx + i * step) % _table.size();
            if (_table[probe].status == EMPTY) break;
            if (_table[probe].status == OCCUPIED && _table[probe].key == key) {
                _table[probe].status = DELETED;
                _size--;
                return;
            }
        }
        throw TableException("Key not found");
    }

    TVal find(const TKey& key) const override {
        size_t idx = hash1(key);
        size_t step = hash2(key);
        for (size_t i = 0; i < _table.size(); ++i) {
            size_t probe = (idx + i * step) % _table.size();
            if (_table[probe].status == EMPTY) break;
            if (_table[probe].status == OCCUPIED && _table[probe].key == key) return _table[probe].val;
        }
        throw TableException("Key not found");
    }

    bool contains(const TKey& key) const override {
        size_t idx = hash1(key);
        size_t step = hash2(key);
        for (size_t i = 0; i < _table.size(); ++i) {
            size_t probe = (idx + i * step) % _table.size();
            if (_table[probe].status == EMPTY) return false;
            if (_table[probe].status == OCCUPIED && _table[probe].key == key) return true;
        }
        return false;
    }

    void clear() override {
        for (auto& e : _table) e.status = EMPTY;
        _size = 0;
    }

    bool isEmpty() const override { return _size == 0; }
    int size() const override { return _size; }

    std::vector<TKey> getKeys() const override {
        std::vector<TKey> keys;
        keys.reserve(_size);
        for (const auto& e : _table) if (e.status == OCCUPIED) keys.push_back(e.key);
        return keys;
    }

    std::vector<TVal> getValues() const override {
        std::vector<TVal> vals;
        vals.reserve(_size);
        for (const auto& e : _table) if (e.status == OCCUPIED) vals.push_back(e.val);
        return vals;
    }

    TVal& operator[](const TKey& key) override {
        if (!contains(key)) insert(key, TVal());

        // После вызова insert таблица могла быть перехеширована, но это безопасно
        // так как вычисляем idx и step заново 
        size_t idx = hash1(key);
        size_t step = hash2(key);
        for (size_t i = 0; i < _table.size(); ++i) {
            size_t probe = (idx + i * step) % _table.size();
            if (_table[probe].status == OCCUPIED && _table[probe].key == key) return _table[probe].val;
        }
        throw TableException("Insert failed internally");
    }

    const TVal& operator[](const TKey& key) const override { return find(key); }

    std::string serialize() const override {
        std::stringstream ss;
        ss << size() << "\n";
        for (const auto& e : _table) {
            if (e.status == OCCUPIED) {
                ss << e.key << "\n" << e.val << "\n";
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

#endif // HASH_TABLE_DOUBLE_H