#ifndef TREETABLE_H
#define TREETABLE_H

#include "itable.h"
#include <string>
#include <sstream>
#include <functional>
#include <algorithm>
#include <memory>
#include <limits>
#include <type_traits>

template<typename TKey, typename TVal>
class TreeTable : public ITable<TKey, TVal> {
private:
    struct Node {
        TKey key;
        TVal value;
        Node* left;
        Node* right;

        Node(const TKey& k, const TVal& v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node* root;
    int count;

    // Вспомогательный метод для преобразования ключа в строку (для сообщений об ошибках)
    std::string keyToString(const TKey& key) const {
        std::stringstream ss;
        ss << key;
        return ss.str();
    }

    Node* insertNode(Node* node, const TKey& key, const TVal& value) {
        if (node == nullptr) {
            count++;
            return new Node(key, value);
        }

        if (key < node->key) {
            node->left = insertNode(node->left, key, value);
        }
        else if (key > node->key) {
            node->right = insertNode(node->right, key, value);
        }
        else {
            // Ключ уже существует, обновляем значение
            node->value = value;
        }

        return node;
    }

    Node* findMin(Node* node) const {
        if (node == nullptr) return nullptr;
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node* removeNode(Node* node, const TKey& key) {
        if (node == nullptr) {
            throw TableException("Key not found");
        }

        if (key < node->key) {
            node->left = removeNode(node->left, key);
        }
        else if (key > node->key) {
            node->right = removeNode(node->right, key);
        }
        else {
            // Нашли узел для удаления

            // Случай 1: лист (нет детей)
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                count--;
                return nullptr;
            }

            // Случай 2: только правый ребенок
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                count--;
                return temp;
            }

            // Случай 3: только левый ребенок
            if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                count--;
                return temp;
            }

            // Случай 4: два ребенка
            // Находим максимальный элемент в левом поддереве
            Node* maxNode = node->left;
            while (maxNode->right != nullptr) {
                maxNode = maxNode->right;
            }

            // Копируем данные из maxNode в текущий узел
            node->key = maxNode->key;
            node->value = maxNode->value;

            // Удаляем maxNode из левого поддерева
            node->left = removeMax(node->left);
        }

        return node;
    }

    Node* removeMax(Node* node) {
        if (node == nullptr) return nullptr;

        if (node->right == nullptr) {
            Node* leftChild = node->left;
            delete node;
            count--;
            return leftChild;
        }

        node->right = removeMax(node->right);
        return node;
    }

    Node* findNode(Node* node, const TKey& key) const {
        if (node == nullptr) return nullptr;

        if (key < node->key) {
            return findNode(node->left, key);
        }
        else if (key > node->key) {
            return findNode(node->right, key);
        }
        else {
            return node;
        }
    }

    void clearTree(Node* node) {
        if (node == nullptr) return;
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }

    void collectKeys(Node* node, std::vector<TKey>& keys) const {
        if (node == nullptr) return;
        collectKeys(node->left, keys);
        keys.push_back(node->key);
        collectKeys(node->right, keys);
    }

    void collectValues(Node* node, std::vector<TVal>& values) const {
        if (node == nullptr) return;
        collectValues(node->left, values);
        values.push_back(node->value);
        collectValues(node->right, values);
    }

    void collectPairs(Node* node, std::vector<std::pair<TKey, TVal>>& pairs) const {
        if (node == nullptr) return;
        collectPairs(node->left, pairs);
        pairs.push_back({ node->key, node->value });
        collectPairs(node->right, pairs);
    }

    // In-order обход для сериализации
    void serializeNode(Node* node, std::stringstream& ss) const {
        if (node == nullptr) return;
        serializeNode(node->left, ss);
        ss << node->key << ":" << node->value << ";";
        serializeNode(node->right, ss);
    }

    // Копирование дерева
    Node* copyTree(Node* node) {
        if (node == nullptr) return nullptr;
        Node* newNode = new Node(node->key, node->value);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        return newNode;
    }

    // Рекурсивная проверка BST с использованием шаблонных параметров
    bool isBSTUtil(Node* node, const TKey* min, const TKey* max) const {
        if (node == nullptr) return true;

        // Проверяем текущий узел с min и max
        if (min != nullptr && node->key <= *min) return false;
        if (max != nullptr && node->key >= *max) return false;

        // Рекурсивно проверяем левое и правое поддеревья
        return isBSTUtil(node->left, min, &node->key) &&
            isBSTUtil(node->right, &node->key, max);
    }

public:
    TreeTable() : root(nullptr), count(0) {}

    TreeTable(const TreeTable& other) : root(nullptr), count(0) {
        root = copyTree(other.root);
        count = other.count;
    }

    TreeTable& operator=(const TreeTable& other) {
        if (this != &other) {
            clear();
            root = copyTree(other.root);
            count = other.count;
        }
        return *this;
    }

    ~TreeTable() {
        clear();
    }

    // Основные операции
    void insert(const TKey& key, const TVal& value) override {
        root = insertNode(root, key, value);
    }

    void remove(const TKey& key) override {
        if (root == nullptr) {
            throw TableException("Table is empty");
        }
        root = removeNode(root, key);
    }

    TVal find(const TKey& key) const override {
        Node* node = findNode(root, key);
        if (node == nullptr) {
            throw TableException("Key not found: " + keyToString(key));
        }
        return node->value;
    }

    bool contains(const TKey& key) const override {
        return findNode(root, key) != nullptr;
    }

    void clear() override {
        clearTree(root);
        root = nullptr;
        count = 0;
    }

    bool isEmpty() const override {
        return count == 0;
    }

    int size() const override {
        return count;
    }

    std::vector<TKey> getKeys() const override {
        std::vector<TKey> keys;
        collectKeys(root, keys);
        return keys;
    }

    std::vector<TVal> getValues() const override {
        std::vector<TVal> values;
        collectValues(root, values);
        return values;
    }

    TVal& operator[](const TKey& key) override {
        Node* node = findNode(root, key);
        if (node == nullptr) {
            // Вставка с значением по умолчанию
            insert(key, TVal());
            node = findNode(root, key);
        }
        return node->value;
    }

    const TVal& operator[](const TKey& key) const override {
        Node* node = findNode(root, key);
        if (node == nullptr) {
            throw TableException("Key not found: " + keyToString(key));
        }
        return node->value;
    }

    std::string serialize() const override {
        std::stringstream ss;
        serializeNode(root, ss);
        return ss.str();
    }

    void deserialize(const std::string& data) override {
        clear();
        if (data.empty()) return;

        std::stringstream ss(data);
        std::string pair;

        while (std::getline(ss, pair, ';')) {
            if (pair.empty()) continue;

            size_t colonPos = pair.find(':');
            if (colonPos != std::string::npos) {
                std::string keyStr = pair.substr(0, colonPos);
                std::string valStr = pair.substr(colonPos + 1);

                std::stringstream keyStream(keyStr);
                std::stringstream valStream(valStr);

                TKey key;
                TVal value;

                keyStream >> key;
                valStream >> value;

                insert(key, value);
            }
        }
    }

    // Специальные методы для дерева

    // Получение высоты дерева
    int getHeight() const {
        return getHeight(root);
    }

    // Проверка, является ли дерево корректным BST
    bool isValidBST() const {
        return isBSTUtil(root, nullptr, nullptr);
    }

    // Вывод таблицы в упорядоченном виде (in-order обход)
    std::string toString() const {
        std::stringstream ss;
        toString(root, ss);
        return ss.str();
    }

    // Печать дерева в виде структуры (для отладки)
    void printTree() const {
        printTree(root, 0, "", false);
    }

private:
    int getHeight(Node* node) const {
        if (node == nullptr) return 0;
        return 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

    void toString(Node* node, std::stringstream& ss) const {
        if (node == nullptr) return;
        toString(node->left, ss);
        ss << node->key << " => " << node->value << std::endl;
        toString(node->right, ss);
    }

    void printTree(Node* node, int level, const std::string& prefix, bool isLeft) const {
        if (node == nullptr) {
            if (level == 0) {
                std::cout << "Empty tree" << std::endl;
            }
            return;
        }

        if (node->right != nullptr) {
            printTree(node->right, level + 1, prefix + (isLeft ? "│   " : "    "), false);
        }

        std::cout << prefix;
        if (level > 0) {
            std::cout << (isLeft ? "└── " : "┌── ");
        }
        std::cout << node->key << ":" << node->value << std::endl;

        if (node->left != nullptr) {
            printTree(node->left, level + 1, prefix + (isLeft ? "    " : "│   "), true);
        }
    }
};

#endif // TREETABLE_H