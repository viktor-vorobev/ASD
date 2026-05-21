#ifndef SORTED_TABLE_RB_H
#define SORTED_TABLE_RB_H
#include "itable.h"
#include "rb_tree.h"

template <class TKey, class TVal>
class SortedTableOnRB : public ITable<TKey, TVal> {
private:
    RBTree<TKey, TVal> _tree;
public:
    void insert(const TKey& key, const TVal& value) override {
        if (!_tree.insert(key, value)) throw TableException("Exists");
    }
    void remove(const TKey& key) override { _tree.erase(key); }
    TVal find(const TKey& key) const override {
        TVal* res = _tree.find(key);
        if (!res) throw TableException("Not found");
        return *res;
    }
    bool contains(const TKey& key) const override { return _tree.find(key) != nullptr; }
    void clear() override { _tree.clear(); }
    bool isEmpty() const override { return _tree.size() == 0; }
    int size() const override { return _tree.size(); }

    std::vector<TKey> getKeys() const override {
        std::vector<TKey> keys;
        _tree.traverseInOrder([&keys](const TKey& k, const TVal&) { keys.push_back(k); });
        return keys;
    }
    std::vector<TVal> getValues() const override { return {}; }
    TVal& operator[](const TKey& key) override { throw TableException("Not implemented"); }
    const TVal& operator[](const TKey& key) const override { return find(key); }
    std::string serialize() const override { return ""; }
    void deserialize(const std::string& data) override {}

    // Специальный метод для демонстрации RB
    void printRB() const { _tree.printColors(); }
};
#endif