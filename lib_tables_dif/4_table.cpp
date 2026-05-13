#ifdef UNORDERED_TREE_TABLE_H 

template<typename TKey, typename TValue>
void UnorderedTreeTable<TKey, TValue>::insert(const TKey& key, const TValue& value) {
    if (contains(key)) {
        throw std::runtime_error("Key already exists in table");
    }
    tree.insert(Record(key, value));
}

template<typename TKey, typename TValue>
void UnorderedTreeTable<TKey, TValue>::erase(const TKey& key) {
    Record* rec = tree.findPtr([&key](const Record& r) { return r.key == key; });
    if (rec == nullptr) {
        throw std::runtime_error("Key not found in table");
    }
    tree.erase(*rec);
}

template<typename TKey, typename TValue>
TValue* UnorderedTreeTable<TKey, TValue>::find(const TKey& key) {
    Record* rec = tree.findPtr([&key](const Record& r) { return r.key == key; });
    if (rec != nullptr) {
        return &rec->value;
    }
    return nullptr;
}

template<typename TKey, typename TValue>
const TValue* UnorderedTreeTable<TKey, TValue>::find(const TKey& key) const {
    const Record* rec = tree.findPtr([&key](const Record& r) { return r.key == key; });
    if (rec != nullptr) {
        return &rec->value;
    }
    return nullptr;
}

template<typename TKey, typename TValue>
bool UnorderedTreeTable<TKey, TValue>::contains(const TKey& key) const {
    return tree.findPtr([&key](const Record& r) { return r.key == key; }) != nullptr;
}

template<typename TKey, typename TValue>
bool UnorderedTreeTable<TKey, TValue>::empty() const {
    return tree.empty();
}

template<typename TKey, typename TValue>
size_t UnorderedTreeTable<TKey, TValue>::size() const {
    return tree.size();
}

template<typename TKey, typename TValue>
void UnorderedTreeTable<TKey, TValue>::clear() {
    tree.clear();
}

template<typename TKey, typename TValue>
TValue& UnorderedTreeTable<TKey, TValue>::operator[](const TKey& key) {
    TValue* val = find(key);
    if (val != nullptr) {
        return *val;
    }
    insert(key, TValue());
    return *find(key);
}

template<typename TKey, typename TValue>
const TValue& UnorderedTreeTable<TKey, TValue>::operator[](const TKey& key) const {
    const TValue* val = find(key);
    if (val != nullptr) {
        return *val;
    }
    throw std::runtime_error("Key not found in table");
}

template<typename TKey, typename TValue>
bool UnorderedTreeTable<TKey, TValue>::operator==(const UnorderedTreeTable& other) const {
    return tree == other.tree;
}

template<typename TKey, typename TValue>
bool UnorderedTreeTable<TKey, TValue>::operator!=(const UnorderedTreeTable& other) const {
    return !(*this == other);
}

template<typename TKey, typename TValue>
std::vector<typename UnorderedTreeTable<TKey, TValue>::Record> UnorderedTreeTable<TKey, TValue>::getAllRecords() const {
    return tree.getAllElements();
}

template<typename TKey, typename TValue>
void UnorderedTreeTable<TKey, TValue>::printTable() const {
    std::cout << "Unordered Tree Table (" << size() << " records):" << std::endl;
    std::vector<Record> records = getAllRecords();
    for (const auto& rec : records) {
        std::cout << "  " << rec << std::endl;
    }
}

template<typename TKey, typename TValue>
std::string UnorderedTreeTable<TKey, TValue>::traversePreOrder() const {
    return tree.traversePreOrder();
}

template<typename TKey, typename TValue>
std::string UnorderedTreeTable<TKey, TValue>::traverseInOrder() const {
    return tree.traverseInOrder();
}

template<typename TKey, typename TValue>
std::string UnorderedTreeTable<TKey, TValue>::traversePostOrder() const {
    return tree.traversePostOrder();
}

template<typename TKey, typename TValue>
std::string UnorderedTreeTable<TKey, TValue>::traverseLevelOrder() const {
    return tree.traverseLevelOrder();
}

template<typename TKey, typename TValue>
void UnorderedTreeTable<TKey, TValue>::printTree() const {
    tree.printTree();
}

#endif // UNORDERED_TREE_TABLE_H