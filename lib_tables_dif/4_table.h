#ifndef UNORDERED_TREE_TABLE_H
#define UNORDERED_TREE_TABLE_H

#include "tree.h"
#include <string>
#include <stdexcept>
#include <vector>
#include <functional>

template<typename TKey, typename TValue>
class UnorderedTreeTable {
public:
    struct Record {
        TKey key;
        TValue value;

        Record() : key(TKey()), value(TValue()) {}
        Record(const TKey& k, const TValue& v) : key(k), value(v) {}

        bool operator==(const Record& other) const {
            return key == other.key;
        }

        bool operator!=(const Record& other) const {
            return key != other.key;
        }

        friend std::ostream& operator<<(std::ostream& os, const Record& rec) {
            os << "(" << rec.key << ": " << rec.value << ")";
            return os;
        }
    };

private:
    BinaryTree<Record> tree;

public:
    UnorderedTreeTable() = default;
    UnorderedTreeTable(const UnorderedTreeTable& other) = default;
    UnorderedTreeTable(UnorderedTreeTable&& other) noexcept = default;
    ~UnorderedTreeTable() = default;

    UnorderedTreeTable& operator=(const UnorderedTreeTable& other) = default;
    UnorderedTreeTable& operator=(UnorderedTreeTable&& other) noexcept = default;

    void insert(const TKey& key, const TValue& value);
    void erase(const TKey& key);
    TValue* find(const TKey& key);
    const TValue* find(const TKey& key) const;
    bool contains(const TKey& key) const;
    bool empty() const;
    size_t size() const;
    void clear();

    TValue& operator[](const TKey& key);
    const TValue& operator[](const TKey& key) const;

    bool operator==(const UnorderedTreeTable& other) const;
    bool operator!=(const UnorderedTreeTable& other) const;

    std::vector<Record> getAllRecords() const;

    void printTable() const;

    std::string traversePreOrder() const;
    std::string traverseInOrder() const;
    std::string traversePostOrder() const;
    std::string traverseLevelOrder() const;

    void printTree() const;
};

#include "4_table.cpp"

#endif // UNORDERED_TREE_TABLE_H