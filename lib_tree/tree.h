#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <iostream>
#include <functional>
#include <stdexcept>
#include <string>
#include <sstream>
#include <queue>
#include <vector>
#include <algorithm>

template<typename T>
class BinaryTree {
public:
    struct Node {
        T data;
        Node* left;
        Node* right;

        Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };

private:
    Node* root;
    size_t node_count;

    void insertAtLevel(const T& value);
    Node* findLastNode() const;
    Node* findParentOfLastNode() const;
    void removeLastNode();
    Node* findNode(const T& value) const;
    Node* findNodeWithParent(const T& value, Node*& parent) const;
    void clearTree(Node* node);
    Node* copyTree(Node* node) const;

    void preOrderTraversal(Node* node, std::function<void(const T&)> visit) const;
    void inOrderTraversal(Node* node, std::function<void(const T&)> visit) const;
    void postOrderTraversal(Node* node, std::function<void(const T&)> visit) const;
    void levelOrderTraversal(std::function<void(const T&)> visit) const;

    void printTreeStructure(Node* node, int level, const std::string& prefix, bool isLeft) const;
    int getHeight(Node* node) const;

public:
    BinaryTree();
    BinaryTree(const BinaryTree& other);
    BinaryTree(BinaryTree&& other) noexcept;
    ~BinaryTree();

    BinaryTree& operator=(const BinaryTree& other);
    BinaryTree& operator=(BinaryTree&& other) noexcept;

    void insert(const T& value);
    void erase(const T& value);
    bool find(const T& value) const;
    T* findPtr(const T& value);
    T* findPtr(std::function<bool(const T&)> predicate);
    const T* findPtr(const T& value) const;
    const T* findPtr(std::function<bool(const T&)> predicate) const;
    std::vector<T> getAllElements() const;
    bool empty() const;
    size_t size() const;
    void clear();

    bool operator==(const BinaryTree& other) const;
    bool operator!=(const BinaryTree& other) const;

    std::string traversePreOrder() const;
    std::string traverseInOrder() const;
    std::string traversePostOrder() const;
    std::string traverseLevelOrder() const;

    void printTree() const;

    class Iterator {
    private:
        Node** stack;
        int stack_capacity;
        int stack_size;
        Node* current;

        void pushLeftBranch(Node* node);

    public:
        Iterator(Node* start);
        Iterator();  // Äëÿ end()
        ~Iterator();
        Iterator(const Iterator& other);
        Iterator& operator=(const Iterator& other);

        const T& operator*() const;
        T& operator*();
        Iterator& operator++();
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
    };

    Iterator begin();
    Iterator end() const;
};


#endif // BINARY_TREE_H

#include "tree.cpp"