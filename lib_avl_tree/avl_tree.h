#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <algorithm>
#include <functional>
#include <utility>

template <typename K, typename V>
class AVLTree {
private:
    struct Node {
        K key;
        V val;
        int height;
        Node* left;
        Node* right;

        // Конструкторы с обязательным занулением указателей
        Node(const K& k, const V& v)
            : key(k), val(v), height(1), left(nullptr), right(nullptr) {
        }

        Node(K&& k, V&& v)
            : key(std::move(k)), val(std::move(v)), height(1), left(nullptr), right(nullptr) {
        }
    };

    Node* root = nullptr;
    int _size = 0;

    int height(Node* N) const { return N ? N->height : 0; }
    int getBalance(Node* N) const { return N ? height(N->left) - height(N->right) : 0; }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        return y;
    }

    Node* copyHelper(Node* n) {
        if (!n) return nullptr;
        Node* newNode = new Node(n->key, n->val);
        newNode->height = n->height;
        newNode->left = copyHelper(n->left);
        newNode->right = copyHelper(n->right);
        return newNode;
    }

    Node* insertHelper(Node* node, const K& key, const V& val, bool& inserted) {
        if (!node) { inserted = true; return new Node(key, val); }
        if (key < node->key) node->left = insertHelper(node->left, key, val, inserted);
        else if (key > node->key) node->right = insertHelper(node->right, key, val, inserted);
        else return node;

        node->height = 1 + std::max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key) return rightRotate(node);
        if (balance < -1 && key > node->right->key) return leftRotate(node);
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    Node* insertHelper(Node* node, K&& key, V&& val, bool& inserted) {
        if (!node) { inserted = true; return new Node(std::move(key), std::move(val)); }
        if (key < node->key) node->left = insertHelper(node->left, std::move(key), std::move(val), inserted);
        else if (key > node->key) node->right = insertHelper(node->right, std::move(key), std::move(val), inserted);
        else return node;

        node->height = 1 + std::max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key) return rightRotate(node);
        if (balance < -1 && key > node->right->key) return leftRotate(node);
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr) current = current->left;
        return current;
    }

    Node* eraseHelper(Node* rootNode, const K& key, bool& erased) {
        if (!rootNode) return rootNode;
        if (key < rootNode->key) rootNode->left = eraseHelper(rootNode->left, key, erased);
        else if (key > rootNode->key) rootNode->right = eraseHelper(rootNode->right, key, erased);
        else {
            erased = true;
            if (!rootNode->left || !rootNode->right) {
                Node* temp = rootNode->left ? rootNode->left : rootNode->right;
                if (!temp) {
                    temp = rootNode;
                    rootNode = nullptr;
                }
                else {
                    Node* toDelete = rootNode;
                    rootNode = temp;
                    temp = toDelete;
                }
                delete temp;
            }
            else {
                Node* temp = minValueNode(rootNode->right);
                rootNode->key = std::move(temp->key);
                rootNode->val = std::move(temp->val);
                rootNode->right = eraseHelper(rootNode->right, temp->key, erased);
            }
        }
        if (!rootNode) return rootNode;

        rootNode->height = 1 + std::max(height(rootNode->left), height(rootNode->right));
        int balance = getBalance(rootNode);

        if (balance > 1 && getBalance(rootNode->left) >= 0) return rightRotate(rootNode);
        if (balance > 1 && getBalance(rootNode->left) < 0) {
            rootNode->left = leftRotate(rootNode->left);
            return rightRotate(rootNode);
        }
        if (balance < -1 && getBalance(rootNode->right) <= 0) return leftRotate(rootNode);
        if (balance < -1 && getBalance(rootNode->right) > 0) {
            rootNode->right = rightRotate(rootNode->right);
            return leftRotate(rootNode);
        }
        return rootNode;
    }

    Node* findNode(Node* n, const K& key) const {
        if (!n) return nullptr;
        if (n->key == key) return n;
        if (key < n->key) return findNode(n->left, key);
        return findNode(n->right, key);
    }

    void inOrderHelper(Node* n, std::function<void(const K&, const V&)> fn) const {
        if (!n) return;
        inOrderHelper(n->left, fn);
        fn(n->key, n->val);
        inOrderHelper(n->right, fn);
    }

    void clearHelper(Node* n) {
        if (!n) return;
        clearHelper(n->left);
        clearHelper(n->right);
        delete n;
    }

public:
    AVLTree() = default;
    ~AVLTree() { clear(); }

    // Конструктор копирования
    AVLTree(const AVLTree& other) {
        root = copyHelper(other.root);
        _size = other._size;
    }

    // Оператор копирующего присваивания
    AVLTree& operator=(const AVLTree& other) {
        if (this != &other) {
            clear();
            root = copyHelper(other.root);
            _size = other._size;
        }
        return *this;
    }

    // Конструктор перемещения
    AVLTree(AVLTree&& other) noexcept : root(other.root), _size(other._size) {
        other.root = nullptr;
        other._size = 0;
    }

    // Оператор перемещающего присваивания
    AVLTree& operator=(AVLTree&& other) noexcept {
        if (this != &other) {
            clear();
            root = other.root;
            _size = other._size;
            other.root = nullptr;
            other._size = 0;
        }
        return *this;
    }

    bool insert(const K& key, const V& val) {
        bool inserted = false;
        root = insertHelper(root, key, val, inserted);
        if (inserted) _size++;
        return inserted;
    }

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

#endif // AVL_TREE_H