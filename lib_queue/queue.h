#ifndef QUEUE_H
#define QUEUE_H

template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* frontNode;
    Node* rearNode;
    size_t queueSize;

public:
    //  онструкторы и деструктор
    Queue();
    Queue(const Queue& other);
    Queue& operator=(const Queue& other);
    ~Queue();

    // ќсновные операции
    void enqueue(const T& value);
    void dequeue();
    T& front();
    const T& front() const;
    bool empty() const;
    size_t size() const;

    // ƒополнительные методы
    void clear();
    void display() const;
};

// явна€ инстанциаци€ шаблона дл€ распространенных типов
template class Queue<int>;
template class Queue<double>;

#endif // QUEUE_H