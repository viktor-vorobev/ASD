#ifndef RB_TREE_H
#define RB_TREE_H

#include <iostream>
#include <functional>

/*
    RB Tree Properties:
    1. Узел RED или BLACK.
    2. Корень BLACK.
    3. Листья (NIL) BLACK.
    4. У RED узла оба потомка BLACK.
    5. Все пути от узла к листьям содержат одинаковое количество BLACK узлов.
*/

enum Color { RED, BLACK };

template <typename K, typename V>
class RBTree {
private:
    struct Node {
        K key; V val; Color color;
        Node* left; Node* right; Node* parent;
        Node(K k, V v) : key(k), val(v), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };
    Node* root = nullptr;
    int _size = 0;

    void rotateLeft(Node*& root, Node*& pt) {
        Node* pt_right = pt->right;
        pt->right = pt_right->left;
        if (pt->right != nullptr) pt->right->parent = pt;
        pt_right->parent = pt->parent;
        if (pt->parent == nullptr) root = pt_right;
        else if (pt == pt->parent->left) pt->parent->left = pt_right;
        else pt->parent->right = pt_right;
        pt_right->left = pt;
        pt->parent = pt_right;
    }

    void rotateRight(Node*& root, Node*& pt) {
        Node* pt_left = pt->left;
        pt->left = pt_left->right;
        if (pt->left != nullptr) pt->left->parent = pt;
        pt_left->parent = pt->parent;
        if (pt->parent == nullptr) root = pt_left;
        else if (pt == pt->parent->left) pt->parent->left = pt_left;
        else pt->parent->right = pt_left;
        pt_left->right = pt;
        pt->parent = pt_left;
    }

    void fixInsert(Node*& root, Node*& pt) {
        Node* parent_pt = nullptr;
        Node* grand_parent_pt = nullptr;
        while ((pt != root) && (pt->color != BLACK) && (pt->parent->color == RED)) {
            parent_pt = pt->parent;
            grand_parent_pt = pt->parent->parent;
            if (parent_pt == grand_parent_pt->left) {
                Node* uncle_pt = grand_parent_pt->right;
                if (uncle_pt != nullptr && uncle_pt->color == RED) {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                }
                else {
                    if (pt == parent_pt->right) {
                        rotateLeft(root, parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateRight(root, grand_parent_pt);
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
            else {
                Node* uncle_pt = grand_parent_pt->left;
                if ((uncle_pt != nullptr) && (uncle_pt->color == RED)) {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                }
                else {
                    if (pt == parent_pt->left) {
                        rotateRight(root, parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateLeft(root, grand_parent_pt);
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
        }
        root->color = BLACK;
    }

    // Для экономии места реализована только вставка (базовый вариант "хорошо"). 
    // Метод erase выбрасывает исключение как нереализованный.

    Node* BSTInsert(Node* root, Node* pt) {
        if (root == nullptr) return pt;
        if (pt->key < root->key) {
            root->left = BSTInsert(root->left, pt);
            root->left->parent = root;
        }
        else if (pt->key > root->key) {
            root->right = BSTInsert(root->right, pt);
            root->right->parent = root;
        }
        return root;
    }

    void inOrderHelper(Node* n, std::function<void(const K&, const V&)> fn) const {
        if (!n) return;
        inOrderHelper(n->left, fn);
        fn(n->key, n->val);
        inOrderHelper(n->right, fn);
    }

    Node* findNode(Node* n, const K& key) const {
        if (!n || n->key == key) return n;
        if (key < n->key) return findNode(n->left, key);
        return findNode(n->right, key);
    }

    void printTreeHelper(Node* root, std::string indent, bool last) const {
        if (root != nullptr) {
            std::cout << indent;
            if (last) { std::cout << "R----"; indent += "     "; }
            else { std::cout << "L----"; indent += "|    "; }
            // ANSI Цвета: 31=Red, 30=Black
            std::string sColor = root->color == RED ? "\033[31mRED\033[0m" : "\033[30;1mBLACK\033[0m";
            std::cout << root->key << "(" << sColor << ")" << std::endl;
            printTreeHelper(root->left, indent, false);
            printTreeHelper(root->right, indent, true);
        }
    }

public:
    bool insert(const K& key, const V& val) {
        if (findNode(root, key)) return false;
        Node* pt = new Node(key, val);
        root = BSTInsert(root, pt);
        fixInsert(root, pt);
        _size++;
        return true;
    }
    bool erase(const K& key) { throw std::runtime_error("RB Erase not fully implemented"); }
    V* find(const K& key) const {
        Node* res = findNode(root, key);
        return res ? &(res->val) : nullptr;
    }
    int size() const { return _size; }
    void clear() { root = nullptr; _size = 0; } // Упрощенно
    void traverseInOrder(std::function<void(const K&, const V&)> fn) const { inOrderHelper(root, fn); }
    void printColors() const { printTreeHelper(root, "", true); }
};
#endif