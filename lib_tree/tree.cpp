#include <queue>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <sstream>

#ifdef BINARY_TREE_H 

template<typename T>
BinaryTree<T>::BinaryTree() : root(nullptr), node_count(0) {}

template<typename T>
BinaryTree<T>::BinaryTree(const BinaryTree& other) : root(nullptr), node_count(0) {
    root = copyTree(other.root);
    node_count = other.node_count;
}

template<typename T>
BinaryTree<T>::BinaryTree(BinaryTree&& other) noexcept : root(other.root), node_count(other.node_count) {
    other.root = nullptr;
    other.node_count = 0;
}

template<typename T>
BinaryTree<T>::~BinaryTree() {
    clear();
}

template<typename T>
BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree& other) {
    if (this != &other) {
        clear();
        root = copyTree(other.root);
        node_count = other.node_count;
    }
    return *this;
}

template<typename T>
BinaryTree<T>& BinaryTree<T>::operator=(BinaryTree&& other) noexcept {
    if (this != &other) {
        clear();
        root = other.root;
        node_count = other.node_count;
        other.root = nullptr;
        other.node_count = 0;
    }
    return *this;
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::copyTree(Node* node) const {
    if (node == nullptr) return nullptr;
    Node* new_node = new Node(node->data);
    new_node->left = copyTree(node->left);
    new_node->right = copyTree(node->right);
    return new_node;
}

template<typename T>
void BinaryTree<T>::clearTree(Node* node) {
    if (node == nullptr) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

template<typename T>
void BinaryTree<T>::clear() {
    clearTree(root);
    root = nullptr;
    node_count = 0;
}

template<typename T>
bool BinaryTree<T>::empty() const {
    return node_count == 0;
}

template<typename T>
size_t BinaryTree<T>::size() const {
    return node_count;
}

template<typename T>
void BinaryTree<T>::insertAtLevel(const T& value) {
    Node* new_node = new Node(value);
    node_count++;

    if (root == nullptr) {
        root = new_node;
        return;
    }

    // BFS для поиска места вставки
    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current->left == nullptr) {
            current->left = new_node;
            return;
        }
        else if (current->right == nullptr) {
            current->right = new_node;
            return;
        }
        else {
            q.push(current->left);
            q.push(current->right);
        }
    }
}

template<typename T>
void BinaryTree<T>::insert(const T& value) {
    insertAtLevel(value);
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::findNode(const T& value) const {
    if (root == nullptr) return nullptr;

    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current->data == value) {
            return current;
        }

        if (current->left != nullptr) q.push(current->left);
        if (current->right != nullptr) q.push(current->right);
    }

    return nullptr;
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::findNodeWithParent(const T& value, Node*& parent) const {
    if (root == nullptr) {
        parent = nullptr;
        return nullptr;
    }

    std::queue<std::pair<Node*, Node*>> q;
    q.push({ root, nullptr });

    while (!q.empty()) {
        auto [current, par] = q.front();
        q.pop();

        if (current->data == value) {
            parent = par;
            return current;
        }

        if (current->left != nullptr) q.push({ current->left, current });
        if (current->right != nullptr) q.push({ current->right, current });
    }

    parent = nullptr;
    return nullptr;
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::findLastNode() const {
    if (root == nullptr) return nullptr;

    Node* last = nullptr;
    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        last = q.front();
        q.pop();

        if (last->left != nullptr) q.push(last->left);
        if (last->right != nullptr) q.push(last->right);
    }

    return last;
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::findParentOfLastNode() const {
    if (root == nullptr || node_count <= 1) return nullptr;

    Node* last = findLastNode();
    if (last == nullptr) return nullptr;

    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current->left == last || current->right == last) {
            return current;
        }

        if (current->left != nullptr) q.push(current->left);
        if (current->right != nullptr) q.push(current->right);
    }

    return nullptr;
}

template<typename T>
void BinaryTree<T>::removeLastNode() {
    if (root == nullptr) return;

    if (node_count == 1) {
        delete root;
        root = nullptr;
        node_count = 0;
        return;
    }

    Node* last = findLastNode();
    Node* parent = findParentOfLastNode();

    if (parent == nullptr || last == nullptr) return;

    if (parent->right == last) {
        parent->right = nullptr;
    }
    else if (parent->left == last) {
        parent->left = nullptr;
    }

    delete last;
    node_count--;
}

template<typename T>
void BinaryTree<T>::erase(const T& value) {
    if (root == nullptr) {
        throw std::runtime_error("Tree is empty");
    }

    Node* parent = nullptr;
    Node* target = findNodeWithParent(value, parent);

    if (target == nullptr) {
        throw std::runtime_error("Element not found in tree");
    }

    Node* last = findLastNode();
    if (last == nullptr) return;

    if (target == last) {
        removeLastNode();
        return;
    }

    T lastData = last->data;
    removeLastNode();
    target->data = lastData;
}

template<typename T>
bool BinaryTree<T>::find(const T& value) const {
    return findNode(value) != nullptr;
}

template<typename T>
T* BinaryTree<T>::findPtr(const T& value) {
    Node* node = findNode(value);
    return node ? &node->data : nullptr;
}

template<typename T>
T* BinaryTree<T>::findPtr(std::function<bool(const T&)> predicate) {
    if (root == nullptr) return nullptr;

    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (predicate(current->data)) {
            return &current->data;
        }

        if (current->left != nullptr) q.push(current->left);
        if (current->right != nullptr) q.push(current->right);
    }

    return nullptr;
}

template<typename T>
const T* BinaryTree<T>::findPtr(const T& value) const {
    Node* node = findNode(value);
    return node ? &node->data : nullptr;
}

template<typename T>
const T* BinaryTree<T>::findPtr(std::function<bool(const T&)> predicate) const {
    if (root == nullptr) return nullptr;

    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (predicate(current->data)) {
            return &current->data;
        }

        if (current->left != nullptr) q.push(current->left);
        if (current->right != nullptr) q.push(current->right);
    }

    return nullptr;
}

template<typename T>
std::vector<T> BinaryTree<T>::getAllElements() const {
    std::vector<T> result;
    levelOrderTraversal([&](const T& val) { result.push_back(val); });
    return result;
}

template<typename T>
bool BinaryTree<T>::operator==(const BinaryTree& other) const {
    if (node_count != other.node_count) return false;

    std::vector<T> this_elements = getAllElements();
    std::vector<T> other_elements = other.getAllElements();

    return this_elements == other_elements;
}

template<typename T>
bool BinaryTree<T>::operator!=(const BinaryTree& other) const {
    return !(*this == other);
}

// Обходы
template<typename T>
void BinaryTree<T>::preOrderTraversal(Node* node, std::function<void(const T&)> visit) const {
    if (node == nullptr) return;
    visit(node->data);
    preOrderTraversal(node->left, visit);
    preOrderTraversal(node->right, visit);
}

template<typename T>
void BinaryTree<T>::inOrderTraversal(Node* node, std::function<void(const T&)> visit) const {
    if (node == nullptr) return;
    inOrderTraversal(node->left, visit);
    visit(node->data);
    inOrderTraversal(node->right, visit);
}

template<typename T>
void BinaryTree<T>::postOrderTraversal(Node* node, std::function<void(const T&)> visit) const {
    if (node == nullptr) return;
    postOrderTraversal(node->left, visit);
    postOrderTraversal(node->right, visit);
    visit(node->data);
}

template<typename T>
void BinaryTree<T>::levelOrderTraversal(std::function<void(const T&)> visit) const {
    if (root == nullptr) return;

    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        visit(current->data);

        if (current->left != nullptr) q.push(current->left);
        if (current->right != nullptr) q.push(current->right);
    }
}

template<typename T>
std::string BinaryTree<T>::traversePreOrder() const {
    std::ostringstream oss;
    bool first = true;

    preOrderTraversal(root, [&](const T& val) {
        if (!first) oss << " ";
        oss << val;
        first = false;
        });

    return oss.str();
}

template<typename T>
std::string BinaryTree<T>::traverseInOrder() const {
    std::ostringstream oss;
    bool first = true;

    inOrderTraversal(root, [&](const T& val) {
        if (!first) oss << " ";
        oss << val;
        first = false;
        });

    return oss.str();
}

template<typename T>
std::string BinaryTree<T>::traversePostOrder() const {
    std::ostringstream oss;
    bool first = true;

    postOrderTraversal(root, [&](const T& val) {
        if (!first) oss << " ";
        oss << val;
        first = false;
        });

    return oss.str();
}

template<typename T>
std::string BinaryTree<T>::traverseLevelOrder() const {
    std::ostringstream oss;
    bool first = true;

    levelOrderTraversal([&](const T& val) {
        if (!first) oss << " ";
        oss << val;
        first = false;
        });

    return oss.str();
}

// Вывод дерева
template<typename T>
int BinaryTree<T>::getHeight(Node* node) const {
    if (node == nullptr) return 0;
    return 1 + std::max(getHeight(node->left), getHeight(node->right));
}

template<typename T>
void BinaryTree<T>::printTreeStructure(Node* node, int level, const std::string& prefix, bool isLeft) const {
    if (node == nullptr) return;

    if (node->right != nullptr) {
        printTreeStructure(node->right, level + 1, prefix + (isLeft ? "│   " : "    "), false);
    }

    std::cout << prefix;
    if (level > 0) {
        std::cout << (isLeft ? "└── " : "┌── ");
    }
    std::cout << node->data << std::endl;

    if (node->left != nullptr) {
        printTreeStructure(node->left, level + 1, prefix + (isLeft ? "    " : "│   "), true);
    }
}

template<typename T>
void BinaryTree<T>::printTree() const {
    if (root == nullptr) {
        std::cout << "Empty tree" << std::endl;
        return;
    }
    printTreeStructure(root, 0, "", false);
}

// Итератор
template<typename T>
BinaryTree<T>::Iterator::Iterator(Node* start) : stack_capacity(16), stack_size(0), current(nullptr) {
    stack = new Node * [stack_capacity];
    pushLeftBranch(start);
    if (stack_size > 0) {
        current = stack[stack_size - 1];
    }
}

template<typename T>
BinaryTree<T>::Iterator::Iterator() : stack_capacity(16), stack_size(0), current(nullptr) {
    stack = new Node * [stack_capacity];
}

template<typename T>
BinaryTree<T>::Iterator::~Iterator() {
    delete[] stack;
}

template<typename T>
BinaryTree<T>::Iterator::Iterator(const Iterator& other)
    : stack_capacity(other.stack_capacity), stack_size(other.stack_size), current(other.current) {
    stack = new Node * [stack_capacity];
    for (int i = 0; i < stack_size; i++) {
        stack[i] = other.stack[i];
    }
}

template<typename T>
typename BinaryTree<T>::Iterator& BinaryTree<T>::Iterator::operator=(const Iterator& other) {
    if (this != &other) {
        delete[] stack;
        stack_capacity = other.stack_capacity;
        stack_size = other.stack_size;
        current = other.current;
        stack = new Node * [stack_capacity];
        for (int i = 0; i < stack_size; i++) {
            stack[i] = other.stack[i];
        }
    }
    return *this;
}

template<typename T>
void BinaryTree<T>::Iterator::pushLeftBranch(Node* node) {
    while (node != nullptr) {
        if (stack_size >= stack_capacity) {
            stack_capacity *= 2;
            Node** new_stack = new Node * [stack_capacity];
            for (int i = 0; i < stack_size; i++) {
                new_stack[i] = stack[i];
            }
            delete[] stack;
            stack = new_stack;
        }
        stack[stack_size++] = node;
        node = node->left;
    }
}

template<typename T>
const T& BinaryTree<T>::Iterator::operator*() const {
    if (current == nullptr) {
        throw std::runtime_error("Dereferencing end iterator");
    }
    return current->data;
}

template<typename T>
T& BinaryTree<T>::Iterator::operator*() {
    if (current == nullptr) {
        throw std::runtime_error("Dereferencing end iterator");
    }
    return current->data;
}

template<typename T>
typename BinaryTree<T>::Iterator& BinaryTree<T>::Iterator::operator++() {
    if (stack_size == 0) {
        current = nullptr;
        return *this;
    }

    Node* node = stack[--stack_size];
    pushLeftBranch(node->right);

    if (stack_size > 0) {
        current = stack[stack_size - 1];
    }
    else {
        current = nullptr;
    }

    return *this;
}

template<typename T>
bool BinaryTree<T>::Iterator::operator==(const Iterator& other) const {
    return current == other.current;
}

template<typename T>
bool BinaryTree<T>::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

template<typename T>
typename BinaryTree<T>::Iterator BinaryTree<T>::begin() {
    return Iterator(root);
}

template<typename T>
typename BinaryTree<T>::Iterator BinaryTree<T>::end() const {
    return Iterator();
}

#endif // BINARY_TREE_H