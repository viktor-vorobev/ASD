#ifndef UNSORTED_TABLE_TREE_H
#define UNSORTED_TABLE_TREE_H

#include "itable.h"
#include "binary_tree.h"
#include <vector>
#include <utility>
#include <sstream>

template <class TKey, class TVal>
class UnsortedTableOnTree : public ITable<TKey, TVal> {
private:
    BinaryTree<std::pair<TKey, TVal>> _tree;

public:
    UnsortedTableOnTree() = default;
    ~UnsortedTableOnTree() override = default;

    void insert(const TKey& key, const TVal& value) override {
        if (contains(key)) throw TableException("Key already exists");
        _tree.insert({ key, value });
    }

    void remove(const TKey& key) override {
        bool removed = _tree.remove([&key](const std::pair<TKey, TVal>& item) { return item.first == key; });
        if (!removed) throw TableException("Key not found");
    }

    TVal find(const TKey& key) const override {
        const auto* res = _tree.find([&key](const std::pair<TKey, TVal>& item) { return item.first == key; });
        if (!res) throw TableException("Key not found");
        return res->second;
    }

    bool contains(const TKey& key) const override {
        return _tree.find([&key](const std::pair<TKey, TVal>& item) { return item.first == key; }) != nullptr;
    }

    void clear() override { _tree.clear(); }
    bool isEmpty() const override { return _tree.empty(); }
    int size() const override { return _tree.size(); }

    std::vector<TKey> getKeys() const override {
        std::vector<TKey> keys;
        _tree.traverseLevelOrder([&keys](const std::pair<TKey, TVal>& item) { keys.push_back(item.first); });
        return keys;
    }

    std::vector<TVal> getValues() const override {
        std::vector<TVal> values;
        _tree.traverseLevelOrder([&values](const std::pair<TKey, TVal>& item) { values.push_back(item.second); });
        return values;
    }

    TVal& operator[](const TKey& key) override {
        auto* res = _tree.find([&key](const std::pair<TKey, TVal>& item) { return item.first == key; });
        if (res) return res->second;
        _tree.insert({ key, TVal() });
        return _tree.find([&key](const std::pair<TKey, TVal>& item) { return item.first == key; })->second;
    }

    const TVal& operator[](const TKey& key) const override {
        const auto* res = _tree.find([&key](const std::pair<TKey, TVal>& item) { return item.first == key; });
        if (!res) throw TableException("Key not found");
        return res->second;
    }

    void printTable() const {
        auto printer = [](const std::pair<TKey, TVal>& item) {
            std::cout << "[" << item.first << " : " << item.second << "] ";
            };
        std::cout << "\n--- Pre-Order Traversal ---\n"; _tree.traversePreOrder(printer);
        std::cout << "\n--- In-Order Traversal ---\n"; _tree.traverseInOrder(printer);
        std::cout << "\n--- Post-Order Traversal ---\n"; _tree.traversePostOrder(printer);
        std::cout << "\n--- Level-Order Traversal ---\n"; _tree.traverseLevelOrder(printer);
        std::cout << "\n--- Beautiful Tree Layout ---\n";
        _tree.printTree([](const std::pair<TKey, TVal>& item) { std::cout << item.first << " (" << item.second << ")"; });
    }

    std::string serialize() const override {
        std::stringstream ss;
        ss << size() << "\n";
        _tree.traverseLevelOrder([&ss](const std::pair<TKey, TVal>& item) {
            ss << item.first << "\n" << item.second << "\n";
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

            if (!std::getline(ss, line)) break;
            std::stringstream keyStream(line);
            keyStream >> key;

            if (!std::getline(ss, line)) break;
            std::stringstream valStream(line);
            valStream >> val;

            _tree.insert({ key, val });
        }
    }
};

#endif // UNSORTED_TABLE_TREE_H