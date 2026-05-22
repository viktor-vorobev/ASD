#ifndef RB_TREE_H
#define RB_TREE_H

#include <iostream>
#include <functional>
#include <stdexcept>
#include <utility>

enum Color { RED, BLACK };

template <typename K, typename V>
class RBTree {
private:
    struct Node {
        K key;
        V val;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(const K& k, const V& v)
            : key(k), val(v), color(RED), left(nullptr), right(nullptr), parent(nullptr) {
        }

        Node(K&& k, V&& v)
            : key(std::move(k)), val(std::move(v)), color(RED), left(nullptr), right(nullptr), parent(nullptr) {
        }
    };

    Node* root = nullptr;
    int _size = 0;

    void clearHelper(Node* n) {
        if (!n) return;
        clearHelper(n->left);
        clearHelper(n->right);
        delete n;
    }

    Node* copyHelper(Node* n, Node* parentNode) {
        if (!n) return nullptr;
        Node* newNode = new Node(n->key, n->val);
        newNode->color = n->color;
        newNode->parent = parentNode;
        try {
            newNode->left = copyHelper(n->left, newNode);
            newNode->right = copyHelper(n->right, newNode);
        }
        catch (...) {
            clearHelper(newNode);
            throw;
        }
        return newNode;
    }

    // Обрати внимание: теперь pt передается по значению (Node*), а не по ссылке, 
    // чтобы не было проблем с передачей временных указателей при удалении.
    void rotateLeft(Node*& rootNode, Node* pt) {
        Node* pt_right = pt->right;
        pt->right = pt_right->left;
        if (pt->right != nullptr) pt->right->parent = pt;
        pt_right->parent = pt->parent;
        if (pt->parent == nullptr) rootNode = pt_right;
        else if (pt == pt->parent->left) pt->parent->left = pt_right;
        else pt->parent->right = pt_right;
        pt_right->left = pt;
        pt->parent = pt_right;
    }

    void rotateRight(Node*& rootNode, Node* pt) {
        Node* pt_left = pt->left;
        pt->left = pt_left->right;
        if (pt->left != nullptr) pt->left->parent = pt;
        pt_left->parent = pt->parent;
        if (pt->parent == nullptr) rootNode = pt_left;
        else if (pt == pt->parent->left) pt->parent->left = pt_left;
        else pt->parent->right = pt_left;
        pt_left->right = pt;
        pt->parent = pt_left;
    }

    void fixInsert(Node*& rootNode, Node* pt) {
        Node* parent_pt = nullptr;
        Node* grand_parent_pt = nullptr;
        while ((pt != rootNode) && (pt->color != BLACK) && (pt->parent->color == RED)) {
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
                        rotateLeft(rootNode, parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateRight(rootNode, grand_parent_pt);
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
                        rotateRight(rootNode, parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateLeft(rootNode, grand_parent_pt);
                    std::swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
        }
        rootNode->color = BLACK;
    }

    Node* BSTInsert(Node* rootNode, Node* pt) {
        if (rootNode == nullptr) return pt;
        if (pt->key < rootNode->key) {
            rootNode->left = BSTInsert(rootNode->left, pt);
            rootNode->left->parent = rootNode;
        }
        else if (pt->key > rootNode->key) {
            rootNode->right = BSTInsert(rootNode->right, pt);
            rootNode->right->parent = rootNode;
        }
        return rootNode;
    }

    // --- НОВЫЙ БЛОК ДЛЯ УДАЛЕНИЯ ---

    Node* minimum(Node* node) {
        while (node->left != nullptr) node = node->left;
        return node;
    }

    // Пересадка поддерева: заменяет поддерево с корнем u на поддерево с корнем v
    void transplant(Node*& rootNode, Node* u, Node* v) {
        if (u->parent == nullptr) rootNode = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        if (v != nullptr) v->parent = u->parent;
    }

    // Балансировка после удаления (восстановление черной высоты)
    void fixDelete(Node* x, Node* x_parent) {
        while (x != root && (x == nullptr || x->color == BLACK)) {
            if (x == x_parent->left) {
                Node* w = x_parent->right;
                if (!w) break; // Защита от поврежденной структуры
                if (w->color == RED) {
                    w->color = BLACK;
                    x_parent->color = RED;
                    rotateLeft(root, x_parent);
                    w = x_parent->right;
                    if (!w) break;
                }
                if ((w->left == nullptr || w->left->color == BLACK) &&
                    (w->right == nullptr || w->right->color == BLACK)) {
                    w->color = RED;
                    x = x_parent;
                    x_parent = x->parent;
                }
                else {
                    if (w->right == nullptr || w->right->color == BLACK) {
                        if (w->left != nullptr) w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(root, w);
                        w = x_parent->right;
                    }
                    w->color = x_parent->color;
                    x_parent->color = BLACK;
                    if (w->right != nullptr) w->right->color = BLACK;
                    rotateLeft(root, x_parent);
                    x = root;
                }
            }
            else {
                Node* w = x_parent->left;
                if (!w) break;
                if (w->color == RED) {
                    w->color = BLACK;
                    x_parent->color = RED;
                    rotateRight(root, x_parent);
                    w = x_parent->left;
                    if (!w) break;
                }
                if ((w->right == nullptr || w->right->color == BLACK) &&
                    (w->left == nullptr || w->left->color == BLACK)) {
                    w->color = RED;
                    x = x_parent;
                    x_parent = x->parent;
                }
                else {
                    if (w->left == nullptr || w->left->color == BLACK) {
                        if (w->right != nullptr) w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(root, w);
                        w = x_parent->left;
                    }
                    w->color = x_parent->color;
                    x_parent->color = BLACK;
                    if (w->left != nullptr) w->left->color = BLACK;
                    rotateRight(root, x_parent);
                    x = root;
                }
            }
        }
        if (x != nullptr) x->color = BLACK;
    }

    // --- КОНЕЦ НОВОГО БЛОКА ---

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

public:
    RBTree() = default;
    ~RBTree() { clear(); }

    RBTree(const RBTree& other) {
        root = copyHelper(other.root, nullptr);
        _size = other._size;
    }

    RBTree& operator=(const RBTree& other) {
        if (this != &other) {
            clear();
            root = copyHelper(other.root, nullptr);
            _size = other._size;
        }
        return *this;
    }

    RBTree(RBTree&& other) noexcept : root(other.root), _size(other._size) {
        other.root = nullptr;
        other._size = 0;
    }

    RBTree& operator=(RBTree&& other) noexcept {
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
        if (findNode(root, key)) return false;
        Node* pt = new Node(key, val);
        root = BSTInsert(root, pt);
        fixInsert(root, pt);
        _size++;
        return true;
    }

    bool insert(K&& key, V&& val) {
        if (findNode(root, key)) return false;
        Node* pt = new Node(std::move(key), std::move(val));
        root = BSTInsert(root, pt);
        fixInsert(root, pt);
        _size++;
        return true;
    }

    // ПОЛНОЦЕННОЕ БЕЗОПАСНОЕ УДАЛЕНИЕ
    bool erase(const K& key) {
        Node* z = root;
        // 1. Поиск узла
        while (z != nullptr) {
            if (key < z->key) z = z->left;
            else if (key > z->key) z = z->right;
            else break;
        }
        if (z == nullptr) return false; // Ключ не найден

        Node* y = z;
        Color y_original_color = y->color;
        Node* x = nullptr;
        Node* x_parent = nullptr; // Критически важно, если x равен nullptr

        // 2. Отсоединение и замена (Transplant)
        if (z->left == nullptr) {
            x = z->right;
            x_parent = z->parent;
            transplant(root, z, z->right);
        }
        else if (z->right == nullptr) {
            x = z->left;
            x_parent = z->parent;
            transplant(root, z, z->left);
        }
        else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x_parent = y;
            }
            else {
                x_parent = y->parent;
                transplant(root, y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(root, z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        // 3. Безопасное уничтожение памяти
        delete z;
        _size--;

        // 4. Восстановление свойств КЧ-дерева, если удалили черный узел
        if (y_original_color == BLACK) {
            fixDelete(x, x_parent);
        }
        return true;
    }

    V* find(const K& key) const {
        Node* res = findNode(root, key);
        return res ? &(res->val) : nullptr;
    }

    int size() const { return _size; }
    bool empty() const { return _size == 0; }

    void clear() {
        clearHelper(root);
        root = nullptr;
        _size = 0;
    }

    void traverseInOrder(std::function<void(const K&, const V&)> fn) const { inOrderHelper(root, fn); }
};

#endif // RB_TREE_H