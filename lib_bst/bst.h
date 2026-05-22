#ifndef BST_H
#define BST_H

#include <iostream>
#include <vector>
#include <functional>
#include <utility>

template <typename K, typename V>
class BST {
protected:
    struct Node {
        K key;
        V val;
        Node* left;
        Node* right;
        Node(const K& k, const V& v) : key(k), val(v), left(nullptr), right(nullptr) {}
        Node(K&& k, V&& v) : key(std::move(k)), val(std::move(v)), left(nullptr), right(nullptr) {}
    };
    Node* root = nullptr;
    int _size = 0;

    void clearHelper(Node* n) {
        if (!n) return;
        clearHelper(n->left);
        clearHelper(n->right);
        delete n;
    }

    // Вспомогательный метод для глубокого копирования узлов поискового дерева
    Node* copyHelper(Node* n) {
        if (!n) return nullptr;
        Node* newNode = new Node(n->key, n->val);
        try {
            newNode->left = copyHelper(n->left);
            newNode->right = copyHelper(n->right);
        }
        catch (...) {
            clearHelper(newNode);
            throw;
        }
        return newNode;
    }

    Node* insertHelper(Node* n, const K& key, const V& val, bool& inserted) {
        if (!n) { inserted = true; return new Node(key, val); }
        if (key < n->key) n->left = insertHelper(n->left, key, val, inserted);
        else if (key > n->key) n->right = insertHelper(n->right, key, val, inserted);
        return n;
    }

    // Перегрузка вставки для rvalue-ссылок (оптимизация перемещения для Polynom)
    Node* insertHelper(Node* n, K&& key, V&& val, bool& inserted) {
        if (!n) { inserted = true; return new Node(std::move(key), std::move(val)); }
        if (key < n->key) n->left = insertHelper(n->left, std::move(key), std::move(val), inserted);
        else if (key > n->key) n->right = insertHelper(n->right, std::move(key), std::move(val), inserted);
        return n;
    }

    Node* eraseHelper(Node* n, const K& key, bool& erased) {
        if (!n) return nullptr;
        if (key < n->key) n->left = eraseHelper(n->left, key, erased);
        else if (key > n->key) n->right = eraseHelper(n->right, key, erased);
        else {
            erased = true;
            if (!n->left && !n->right) { delete n; return nullptr; }
            else if (!n->left) { Node* temp = n->right; delete n; return temp; }
            else if (!n->right) { Node* temp = n->left; delete n; return temp; }
            else {
                Node* maxLeft = n->left;
                while (maxLeft->right) maxLeft = maxLeft->right;
                n->key = std::move(maxLeft->key);
                n->val = std::move(maxLeft->val);
                n->left = eraseHelper(n->left, maxLeft->key, erased);
            }
        }
        return n;
    }

    Node* findNode(Node* n, const K& key) const {
        if (!n || n->key == key) return n;
        if (key < n->key) return findNode(n->left, key);
        return findNode(n->right, key);
    }

    void inOrderHelper(Node* n, std::function<void(const K&, const V&)> fn) const {
        if (!n) return;
        inOrderHelper(n->left, fn);
        fn(n->key, n->val);
        inOrderHelper(n->right, fn);
    }

public:
    BST() = default;

    // Деструктор
    ~BST() { clear(); }

    // Конструктор копирования (Глубокое копирование)
    BST(const BST& other) {
        root = copyHelper(other.root);
        _size = other._size;
    }

    // Оператор копирующего присваивания
    BST& operator=(const BST& other) {
        if (this != &other) {
            clear();
            root = copyHelper(other.root);
            _size = other._size;
        }
        return *this;
    }

    // Конструктор перемещения
    BST(BST&& other) noexcept : root(other.root), _size(other._size) {
        other.root = nullptr;
        other._size = 0;
    }

    // Оператор перемещающего присваивания
    BST& operator=(BST&& other) noexcept {
        if (this != &other) {
            clear();
            root = other.root;
            _size = other._size;
            other.root = nullptr;
            other._size = 0;
        }
        return *this;
    }

    // Вставка по константной ссылке
    bool insert(const K& key, const V& val) {
        bool inserted = false;
        root = insertHelper(root, key, val, inserted);
        if (inserted) _size++;
        return inserted;
    }

    // Вставка с перемещением (эффективно для Polynom)
    bool insert(K&& key, V&& val) {
        bool inserted = false;
        root = insertHelper(root, std::move(key), std::move(val), inserted);
        if (inserted) _size++;
        return inserted;
    }

    bool erase(const K& key) {
        bool erased = false;
        root = eraseHelper(root, key, erased);
        if (erased) _size--;
        return erased;
    }

    V* find(const K& key) const {
        Node* res = findNode(root, key);
        return res ? &(res->val) : nullptr;
    }

    void clear() { clearHelper(root); root = nullptr; _size = 0; }
    int size() const { return _size; }
    bool empty() const { return _size == 0; }
    void traverseInOrder(std::function<void(const K&, const V&)> fn) const { inOrderHelper(root, fn); }
};

#endif // BST_H