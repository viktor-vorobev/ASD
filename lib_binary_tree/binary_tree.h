#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include "queue.h"
#include <functional>
#include <iostream>
#include <string>
#include <utility>

template <typename T>
struct TreeNode {
    T data;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    TreeNode(const T& val) : data(val) {}
};

template <typename T>
class BinaryTree {
private:
    TreeNode<T>* root = nullptr;
    int _size = 0;

    void clearHelper(TreeNode<T>* node) {
        if (!node) return;
        clearHelper(node->left);
        clearHelper(node->right);
        delete node;
    }

    void preOrder(TreeNode<T>* node, const std::function<void(const T&)>& visit) const {
        if (!node) return;
        visit(node->data);
        preOrder(node->left, visit);
        preOrder(node->right, visit);
    }

    void inOrder(TreeNode<T>* node, const std::function<void(const T&)>& visit) const {
        if (!node) return;
        inOrder(node->left, visit);
        visit(node->data);
        inOrder(node->right, visit);
    }

    void postOrder(TreeNode<T>* node, const std::function<void(const T&)>& visit) const {
        if (!node) return;
        postOrder(node->left, visit);
        postOrder(node->right, visit);
        visit(node->data);
    }

    void printTreeHelper(TreeNode<T>* node, int level, const std::string& type, const std::function<void(const T&)>& printNode) const {
        if (!node) return;
        std::cout << std::string(level * 4, ' ') << type << " ";
        printNode(node->data);
        std::cout << "\n";
        printTreeHelper(node->left, level + 1, "L--->", printNode);
        printTreeHelper(node->right, level + 1, "R--->", printNode);
    }

    // Рекурсивное копирование узлов для конструктора копирования
    TreeNode<T>* copyHelper(TreeNode<T>* node) {
        if (!node) return nullptr;

        TreeNode<T>* newNode = new TreeNode<T>(node->data);
        try {
            newNode->left = copyHelper(node->left);
            newNode->right = copyHelper(node->right);
        }
        catch (...) {
            clearHelper(newNode); 
            throw;
        }
        return newNode;
    }

public:
    BinaryTree() = default;
    ~BinaryTree() { clear(); }

    // Конструктор копирования 
    BinaryTree(const BinaryTree& other) {
        root = copyHelper(other.root);
        _size = other._size;
    }

    // Оператор присваивания
    BinaryTree& operator=(const BinaryTree& other) {
        if (this != &other) {
            clear(); // Очищаем текущее дерево
            root = copyHelper(other.root);
            _size = other._size;
        }
        return *this;
    }

    // Конструктор перемещения 
    BinaryTree(BinaryTree&& other) noexcept : root(other.root), _size(other._size) {
        other.root = nullptr;
        other._size = 0;
    }

    // Оператор перемещения
    BinaryTree& operator=(BinaryTree&& other) noexcept {
        if (this != &other) {
            clear();
            root = other.root;
            _size = other._size;
            other.root = nullptr;
            other._size = 0;
        }
        return *this;
    }

    void insert(const T& val) {
        if (!root) {
            root = new TreeNode<T>(val);
            _size++;
            return;
        }

        Queue<TreeNode<T>*> q;
        q.enqueue(root);

        while (!q.empty()) {
            auto* curr = q.front();
            q.dequeue();

            if (!curr->left) {
                curr->left = new TreeNode<T>(val);
                _size++;
                return;
            }
            else {
                q.enqueue(curr->left);
            }

            if (!curr->right) {
                curr->right = new TreeNode<T>(val);
                _size++;
                return;
            }
            else {
                q.enqueue(curr->right);
            }
        }
    }

    bool remove(const std::function<bool(const T&)>& pred) {
        if (!root) return false;

        TreeNode<T>* targetNode = nullptr;
        // Очередь для хранения пар: текущий узел и его родитель
        Queue<std::pair<TreeNode<T>*, TreeNode<T>*>> pq;
        pq.enqueue({ root, nullptr });

        TreeNode<T>* lastNode = nullptr;
        TreeNode<T>* lastParent = nullptr;

        while (!pq.empty()) {
            auto currentPair = pq.front();
            pq.dequeue();

            TreeNode<T>* curr = currentPair.first;
            TreeNode<T>* parent = currentPair.second;

            if (pred(curr->data)) {
                targetNode = curr;
            }
            lastNode = curr;
            lastParent = parent;

            if (curr->left) pq.enqueue({ curr->left, curr });
            if (curr->right) pq.enqueue({ curr->right, curr });
        }

        if (!targetNode) return false;

        // Перенос данных из последнего узла в удаляемый
        targetNode->data = lastNode->data;

        // Удаляем последний узел физически
        if (lastParent) {
            if (lastParent->left == lastNode) lastParent->left = nullptr;
            else lastParent->right = nullptr;
            delete lastNode;
        }
        else {
            // Если в дереве был всего один корень
            delete root;
            root = nullptr;
        }
        _size--;
        return true;
    }

    T* find(const std::function<bool(const T&)>& pred) {
        if (!root) return nullptr;
        Queue<TreeNode<T>*> q;
        q.enqueue(root);

        while (!q.empty()) {
            auto* curr = q.front();
            q.dequeue();

            if (pred(curr->data)) return &(curr->data);
            if (curr->left) q.enqueue(curr->left);
            if (curr->right) q.enqueue(curr->right);
        }
        return nullptr;
    }

    const T* find(const std::function<bool(const T&)>& pred) const {
        if (!root) return nullptr;
        Queue<TreeNode<T>*> q;
        q.enqueue(root);

        while (!q.empty()) {
            auto* curr = q.front();
            q.dequeue();

            if (pred(curr->data)) return &(curr->data);
            if (curr->left) q.enqueue(curr->left);
            if (curr->right) q.enqueue(curr->right);
        }
        return nullptr;
    }

    void clear() {
        clearHelper(root);
        root = nullptr;
        _size = 0;
    }

    int size() const { return _size; }
    bool empty() const { return _size == 0; }

    void traversePreOrder(const std::function<void(const T&)>& visit) const { preOrder(root, visit); }
    void traverseInOrder(const std::function<void(const T&)>& visit) const { inOrder(root, visit); }
    void traversePostOrder(const std::function<void(const T&)>& visit) const { postOrder(root, visit); }

    void traverseLevelOrder(const std::function<void(const T&)>& visit) const {
        if (!root) return;
        Queue<TreeNode<T>*> q;
        q.enqueue(root);

        while (!q.empty()) {
            auto* curr = q.front();
            q.dequeue();

            visit(curr->data);
            if (curr->left) q.enqueue(curr->left);
            if (curr->right) q.enqueue(curr->right);
        }
    }

    void printTree(const std::function<void(const T&)>& printNode) const {
        printTreeHelper(root, 0, "Root:", printNode);
    }
};

#endif // BINARY_TREE_H