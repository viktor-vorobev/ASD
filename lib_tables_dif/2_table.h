#ifndef SORTED_TABLE_ON_ARR_H
#define SORTED_TABLE_ON_ARR_H

#include "itable.h"
#include "TVector.h"
#include <utility>
#include <string>
#include <vector>
#include <sstream>

template <class TKey, class TVal>
class SortedTableOnArr : public ITable<TKey, TVal> {
private:
    TVector<std::pair<TKey, TVal>> rows;

    int binarySearch(const TKey& key) const {
        int left = 0;
        int right = rows.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (rows[mid].first == key) {
                return mid;
            }
            if (rows[mid].first < key) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        return -1;
    }

    int findInsertPosition(const TKey& key) const {
        int left = 0;
        int right = rows.size();

        while (left < right) {
            int mid = left + (right - left) / 2;
            if (rows[mid].first < key) {
                left = mid + 1;
            }
            else {
                right = mid;
            }
        }
        return left;
    }

    void ensureSorted() const {
        for (int i = 1; i < rows.size(); ++i) {
            if (rows[i - 1].first > rows[i].first) {
                throw TableException("Table is not properly sorted");
            }
        }
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
    SortedTableOnArr() = default;

    SortedTableOnArr(const SortedTableOnArr& other) : rows(other.rows) {
        ensureSorted();
    }

    SortedTableOnArr(SortedTableOnArr&& other) noexcept : rows(std::move(other.rows)) {}

    ~SortedTableOnArr() override = default;

    SortedTableOnArr& operator=(const SortedTableOnArr& other) {
        if (this != &other) {
            rows = other.rows;
            ensureSorted();
        }
        return *this;
    }

    SortedTableOnArr& operator=(SortedTableOnArr&& other) noexcept {
        if (this != &other) {
            rows = std::move(other.rows);
        }
        return *this;
    }

    void insert(const TKey& key, const TVal& value) override {
        int index = binarySearch(key);
        if (index != -1) {
            rows[index].second = value;
        }
        else {
            int pos = findInsertPosition(key);
            rows.insert(pos, std::make_pair(key, value));
        }
    }

    void remove(const TKey& key) override {
        int index = binarySearch(key);
        if (index == -1) {
            throw TableException("Key not found: " + keyToString(key));
        }
        rows.erase(index);
    }

    TVal find(const TKey& key) const override {
        int index = binarySearch(key);
        if (index == -1) {
            throw TableException("Key not found: " + keyToString(key));
        }
        return rows[index].second;
    }

    bool contains(const TKey& key) const override {
        return binarySearch(key) != -1;
    }

    void clear() override {
        rows.clear();
    }

    bool isEmpty() const override {
        return rows.is_empty();
    }

    int size() const override {
        return rows.size();
    }

    std::vector<TKey> getKeys() const override {
        std::vector<TKey> keys;
        for (int i = 0; i < rows.size(); ++i) {
            keys.push_back(rows[i].first);
        }
        return keys;
    }

    std::vector<TVal> getValues() const override {
        std::vector<TVal> values;
        for (int i = 0; i < rows.size(); ++i) {
            values.push_back(rows[i].second);
        }
        return values;
    }

    TVal& operator[](const TKey& key) override {
        int index = binarySearch(key);
        if (index == -1) {
            int pos = findInsertPosition(key);
            rows.insert(pos, std::make_pair(key, TVal()));
            return rows[pos].second;
        }
        return rows[index].second;
    }

    const TVal& operator[](const TKey& key) const override {
        int index = binarySearch(key);
        if (index == -1) {
            throw TableException("Key not found: " + keyToString(key));
        }
        return rows[index].second;
    }

    std::string serialize() const override {
        std::stringstream ss;
        ss << rows.size() << "\n";
        for (int i = 0; i < rows.size(); ++i) {
            ss << rows[i].first << "\n";
            ss << rows[i].second << "\n";
        }
        return ss.str();
    }

    void deserialize(const std::string& data) override {
        clear();
        std::stringstream ss(data);
        int count;
        ss >> count;
        ss.ignore();

        for (int i = 0; i < count; ++i) {
            std::string keyStr, valStr;
            std::getline(ss, keyStr);
            std::getline(ss, valStr);

            TKey key = stringToKey(keyStr);
            TVal val = stringToVal(valStr);

            rows.push_back(std::make_pair(key, val));
        }
    }
};

#endif // SORTED_TABLE_ON_ARR_H