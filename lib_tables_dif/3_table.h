#ifndef UNSORTED_TABLE_ON_LIST_H
#define UNSORTED_TABLE_ON_LIST_H

#include "itable.h"
#include "list.h"
#include <utility>
#include <string>
#include <vector>
#include <sstream>

template <class TKey, class TVal>
class UnsortedTableOnList : public ITable<TKey, TVal> {
private:
    List<std::pair<TKey, TVal>> rows;

    typename List<std::pair<TKey, TVal>>::Iterator findIterator(const TKey& key) {
        for (auto it = rows.begin(); it != rows.end(); ++it) {
            if (it->first == key) {
                return it;
            }
        }
        return rows.end();
    }

    typename List<std::pair<TKey, TVal>>::ConstIterator findIterator(const TKey& key) const {
        for (auto it = rows.cbegin(); it != rows.cend(); ++it) {
            if (it->first == key) {
                return it;
            }
        }
        return rows.cend();
    }

    std::string keyToString(const TKey& key) const {
        std::stringstream ss;
        ss << key;
        return ss.str();
    }

    TKey stringToKey(const std::string& str) const {
        TKey key;
        std::stringstream ss(str);
        ss >> key;
        return key;
    }

    TVal stringToVal(const std::string& str) const {
        TVal val;
        std::stringstream ss(str);
        ss >> val;
        return val;
    }

public:
    UnsortedTableOnList() = default;

    UnsortedTableOnList(const UnsortedTableOnList& other) : rows(other.rows) {}

    UnsortedTableOnList(UnsortedTableOnList&& other) noexcept : rows(std::move(other.rows)) {}

    ~UnsortedTableOnList() override = default;

    UnsortedTableOnList& operator=(const UnsortedTableOnList& other) {
        if (this != &other) {
            rows = other.rows;
        }
        return *this;
    }

    UnsortedTableOnList& operator=(UnsortedTableOnList&& other) noexcept {
        if (this != &other) {
            rows = std::move(other.rows);
        }
        return *this;
    }

    void insert(const TKey& key, const TVal& value) override {
        auto it = findIterator(key);
        if (it != rows.end()) {
            it->second = value;
        }
        else {
            rows.push_back(std::make_pair(key, value));
        }
    }

    void remove(const TKey& key) override {
        auto it = findIterator(key);
        if (it == rows.end()) {
            throw TableException("Key not found: " + keyToString(key));
        }

        int pos = 0;
        for (auto iter = rows.begin(); iter != rows.end(); ++iter) {
            if (iter == it) {
                rows.erase(static_cast<size_t>(pos));
                return;
            }
            ++pos;
        }
    }

    TVal find(const TKey& key) const override {
        auto it = findIterator(key);
        if (it == rows.cend()) {
            throw TableException("Key not found: " + keyToString(key));
        }
        return it->second;
    }

    bool contains(const TKey& key) const override {
        return findIterator(key) != rows.cend();
    }

    void clear() override {
        rows.clear();
    }

    bool isEmpty() const override {
        return rows.empty();
    }

    int size() const override {
        return static_cast<int>(rows.size());
    }

    std::vector<TKey> getKeys() const override {
        std::vector<TKey> keys;
        for (auto it = rows.cbegin(); it != rows.cend(); ++it) {
            keys.push_back(it->first);
        }
        return keys;
    }

    std::vector<TVal> getValues() const override {
        std::vector<TVal> values;
        for (auto it = rows.cbegin(); it != rows.cend(); ++it) {
            values.push_back(it->second);
        }
        return values;
    }

    TVal& operator[](const TKey& key) override {
        auto it = findIterator(key);
        if (it == rows.end()) {
            rows.push_back(std::make_pair(key, TVal()));
            return rows.back().second;
        }
        return it->second;
    }

    const TVal& operator[](const TKey& key) const override {
        auto it = findIterator(key);
        if (it == rows.cend()) {
            throw TableException("Key not found: " + keyToString(key));
        }
        return it->second;
    }

    std::string serialize() const override {
        std::stringstream ss;
        ss << rows.size() << "\n";
        for (auto it = rows.cbegin(); it != rows.cend(); ++it) {
            ss << it->first << "\n";
            // »спользуем оператор << дл€ сериализации значени€
            ss << it->second << "\n";
        }
        return ss.str();
    }

    void deserialize(const std::string& data) override {
        clear();
        std::stringstream ss(data);
        size_t count;
        ss >> count;
        ss.ignore();

        for (size_t i = 0; i < count; ++i) {
            std::string keyStr, valStr;
            std::getline(ss, keyStr);
            std::getline(ss, valStr);

            TKey key = stringToKey(keyStr);
            TVal val = stringToVal(valStr);

            rows.push_back(std::make_pair(key, val));
        }
    }
};

#endif // UNSORTED_TABLE_ON_LIST_H