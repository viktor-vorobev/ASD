#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <algorithm>
#include <functional>

/*
    Схемы поворотов для AVL:

    Правый поворот (LL Case):          Левый поворот (RR Case):
        y                               x
       / \                            /  \
      x   T3   - - - - - - - >      T1   y
     / \       < - - - - - - -          / \
    T1  T2                             T2  T3
*/

template <typename K, typename V>
class AVLTree {
private:
    struct Node {
        K key; V val; int height;
        Node* left; Node* right;
        Node(const K& k, const V& v) : key(k), val(v), height(1), left(nullptr), right(nullptr) {}
    };
    Node* root = nullptr;
    int _size = 0;

    int height(Node* N) { return N ? N->height : 0; }
    int getBalance(Node* N) { return N ? height(N->left) - height(N->right) : 0; }

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

    Node* insertHelper(Node* node, const K& key, const V& val, bool& inserted) {
        if (!node) { inserted = true; return new Node(key, val); }
        if (key < node->key) node->left = insertHelper(node->left, key, val, inserted);
        else if (key > node->key) node->right = insertHelper(node->right, key, val, inserted);
        else return node; // Дубликаты запрещены

        node->height = 1 + std::max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key) return rightRotate(node); // LL
        if (balance < -1 && key > node->right->key) return leftRotate(node); // RR
        if (balance > 1 && key > node->left->key) { // LR
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) { // RL
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

    Node* eraseHelper(Node* root, const K& key, bool& erased) {
        if (!root) return root;
        if (key < root->key) root->left = eraseHelper(root->left, key, erased);
        else if (key > root->key) root->right = eraseHelper(root->right, key, erased);
        else {
            erased = true;
            if (!root->left || !root->right) {
                Node* temp = root->left ? root->left : root->right;
                if (!temp) { temp = root; root = nullptr; }
                else *root = *temp;
                delete temp;
            }
            else {
                Node* temp = minValueNode(root->right);
                root->key = temp->key; root->val = temp->val;
                root->right = eraseHelper(root->right, temp->key, erased);
            }
        }
        if (!root) return root;

        root->height = 1 + std::max(height(root->left), height(root->right));
        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root); // LL
        if (balance > 1 && getBalance(root->left) < 0) { // LR
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root); // RR
        if (balance < -1 && getBalance(root->right) > 0) { // RL
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }

    void inOrderHelper(Node* n, std::function<void(const K&, const V&)> fn) const {
        if (!n) return;
        inOrderHelper(n->left, fn);
        fn(n->key, n->val);
        inOrderHelper(n->right, fn);
    }

    void clearHelper(Node* n) {
        if (!n) return;
        clearHelper(n->left); clearHelper(n->right);
        delete n;
    }

    Node* findNode(Node* n, const K& key) const {
        if (!n || n->key == key) return n;
        if (key < n->key) return findNode(n->left, key);
        return findNode(n->right, key);
    }

public:
    ~AVLTree() { clear(); }
    bool insert(const K& key, const V& val) {
        bool inserted = false;
        root = insertHelper(root, key, val, inserted);
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
    void traverseInOrder(std::function<void(const K&, const V&)> fn) const { inOrderHelper(root, fn); }
};
#endif // AVL_TREE_H