#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>
#include <utility>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include "../lib_list/list.h" 

template<typename TKey, typename TValue>
struct Node {
    std::pair<TKey, TValue> data;
    Node** next;
    size_t level;

    Node(const TKey& key, const TValue& value)
        : data(key, value), next(nullptr), level(0) {
    }

    ~Node() {
        delete[] next; 
    }
};

template<typename TKey, typename TValue>
class SkipList {
private:
    List<Node<TKey, TValue>*> _heads;
    size_t _cur_level;
    size_t _max_level;

    Node<TKey, TValue>*& get_head(size_t level) {
        auto it = _heads.begin();
        for (size_t i = 0; i < level; ++i) {
            ++it;
        }
        return *it;
    }

public:
    SkipList(size_t max_level = 16) : _cur_level(0), _max_level(max_level) {
        for (size_t i = 0; i < _max_level; ++i) {
            _heads.push_back(nullptr);
        }
    }

    ~SkipList() {
        if (_heads.empty()) return;

        Node<TKey, TValue>* curr = get_head(0);
        while (curr != nullptr) {
            Node<TKey, TValue>* next_node = curr->next[0];
            delete curr;
            curr = next_node;
        }
    }

    Node<TKey, TValue>** find_path(const TKey& key) {
        Node<TKey, TValue>** path = new Node<TKey, TValue>* [_max_level];
        for (size_t i = 0; i < _max_level; ++i) {
            path[i] = nullptr;
        }

        Node<TKey, TValue>* curr = nullptr;

        for (int i = static_cast<int>(_max_level) - 1; i >= 0; --i) {
            Node<TKey, TValue>* next_node = (curr == nullptr) ? get_head(i) : curr->next[i];

            while (next_node != nullptr && next_node->data.first < key) {
                curr = next_node;
                next_node = curr->next[i];
            }
            path[i] = curr; 
        }
        return path;
    }

    void insert(const TKey& key, const TValue& value) {
        Node<TKey, TValue>** path = find_path(key);

        Node<TKey, TValue>* next_node = (path[0] == nullptr) ? get_head(0) : path[0]->next[0];
        if (next_node != nullptr && next_node->data.first == key) {
            next_node->data.second = value; 
            delete[] path;
            return;
        }

        size_t random_level = 1;
        while (random_level < _max_level && (std::rand() % 2 == 0)) {
            random_level++;
        }

        if (random_level > _cur_level) {
            _cur_level = random_level;
        }

        Node<TKey, TValue>* new_node = new Node<TKey, TValue>(key, value);
        new_node->level = random_level;
        new_node->next = new Node<TKey, TValue>* [random_level];
        for (size_t i = 0; i < random_level; ++i) {
            new_node->next[i] = nullptr;
        }

        for (size_t i = 0; i < random_level; ++i) {
            Node<TKey, TValue>*& head_ref = get_head(i);
            if (path[i] == nullptr) {
                new_node->next[i] = head_ref;
                head_ref = new_node;
            }
            else {
                new_node->next[i] = path[i]->next[i];
                path[i]->next[i] = new_node;
            }
        }

        delete[] path;
    }

    void print() {
        if (_heads.empty()) return;

        Node<TKey, TValue>* curr = get_head(0);
        while (curr != nullptr) {
            std::stringstream ss;
            ss << curr->data.first << " : " << curr->data.second;

            std::cout << "( " << std::setw(25) << std::left << ss.str() << " ) ";

            for (size_t i = 0; i < curr->level; ++i) {
                std::cout << "[ ] ";
            }
            std::cout << "\n";

            curr = curr->next[0];
        }
    }
};

#endif // SKIPLIST_H