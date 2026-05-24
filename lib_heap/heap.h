#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <stdexcept>
#include <utility>

template <typename T>
class MinHeap {
private:
    std::vector<T> _data;

    size_t getLeftChild(size_t parentIndex) const { return 2 * parentIndex + 1; }
    size_t getRightChild(size_t parentIndex) const { return 2 * parentIndex + 2; }
    size_t getParent(size_t childIndex) const { return (childIndex - 1) / 2; }

    void siftUp(size_t index) {
        while (index > 0 && _data[index] < _data[getParent(index)]) {
            std::swap(_data[index], _data[getParent(index)]);
            index = getParent(index); 
        }
    }

    void siftDown(size_t index) {
        size_t minIndex = index;
        size_t left = getLeftChild(index);
        size_t right = getRightChild(index);

        if (left < _data.size() && _data[left] < _data[minIndex]) {
            minIndex = left;
        }
        if (right < _data.size() && _data[right] < _data[minIndex]) {
            minIndex = right;
        }

        
        if (index != minIndex) {
            std::swap(_data[index], _data[minIndex]);
            siftDown(minIndex); 
        }
    }

public:
    MinHeap() = default;
    ~MinHeap() = default;

    bool empty() const { return _data.empty(); }
    size_t size() const { return _data.size(); }

    void insert(const T& value) {
        _data.push_back(value);    
        siftUp(_data.size() - 1); 
    }

    T pop() {
        if (empty()) {
            throw std::underflow_error("Куча пуста!");
        }

        T rootValue = _data[0];       
        _data[0] = _data.back();    
        _data.pop_back();            
        if (!empty()) {
            siftDown(0);            
        }

        return rootValue;           
    }
};

template <typename T>
std::vector<T> heapSort(const std::vector<T>& input) {
    MinHeap<T> heap;

    for (const auto& item : input) {
        heap.insert(item);
    }

    std::vector<T> sortedData;
    sortedData.reserve(input.size());

    while (!heap.empty()) {
        sortedData.push_back(heap.pop()); 
    }

    return sortedData;
}

#endif // HEAP_H