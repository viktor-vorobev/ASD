#ifndef SORTED_TABLE_BST_H
#define SORTED_TABLE_BST_H

#include "itable.h"
#include "bst.h"
#include <vector>
#include <sstream>
#include <string>

template <class TKey, class TVal>
class SortedTableOnBST : public ITable<TKey, TVal> {
private:
    BST<TKey, TVal> _tree;
public:
    void insert(const TKey& key, const TVal& value) override {
        if (!_tree.insert(key, value)) throw TableException("Key already exists");
    }

    void remove(const TKey& key) override {
        if (!_tree.erase(key)) throw TableException("Key not found");
    }

    TVal find(const TKey& key) const override {
        TVal* res = _tree.find(key);
        if (!res) throw TableException("Key not found");
        return *res;
    }

    bool contains(const TKey& key) const override {
        return _tree.find(key) != nullptr;
    }

    void clear() override { _tree.clear(); }
    bool isEmpty() const override { return _tree.empty(); }
    int size() const override { return _tree.size(); }

    std::vector<TKey> getKeys() const override {
        std::vector<TKey> keys;
        keys.reserve(_tree.size()); // Оптимизация памяти
        _tree.traverseInOrder([&keys](const TKey& k, const TVal&) { keys.push_back(k); });
        return keys;
    }

    std::vector<TVal> getValues() const override {
        std::vector<TVal> vals;
        vals.reserve(_tree.size()); // Оптимизация памяти
        _tree.traverseInOrder([&vals](const TKey&, const TVal& v) { vals.push_back(v); });
        return vals;
    }

    TVal& operator[](const TKey& key) override {
        TVal* res = _tree.find(key);
        if (res) return *res;

        // здесь происходит 3 прохода дерева (O(log N))
        _tree.insert(key, TVal());
        return *(_tree.find(key));
    }

    const TVal& operator[](const TKey& key) const override {
        return find(key);
    }

    std::string serialize() const override {
        std::stringstream ss;
        ss << size() << "\n";
        _tree.traverseInOrder([&ss](const TKey& k, const TVal& v) {
            ss << k << "\n" << v << "\n";
            });
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

            // Безопасное чтение ключа
            if (!std::getline(ss, line)) break;
            std::stringstream keyStream(line);
            keyStream >> key;

            // Безопасное чтение значения
            if (!std::getline(ss, line)) break;
            std::stringstream valStream(line);
            valStream >> val;

            insert(key, val);
        }
    }
};
#endif // SORTED_TABLE_BST_H