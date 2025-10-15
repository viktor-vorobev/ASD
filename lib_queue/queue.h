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

    Node* frontNode;  // Указатель на начало очереди (первый элемент)
    Node* rearNode;   // Указатель на конец очереди (последний элемент)
    size_t queueSize; // Текущий размер очереди

public:
    // Конструкторы и деструктор
    Queue();                           
    Queue(const Queue& other);         
    Queue& operator=(const Queue& other); 
    ~Queue();                          

    // Основные операции очереди
    void enqueue(const T& value);      // Добавление элемента в конец очереди
    void dequeue();                    // Удаление элемента из начала очереди
    T& front();                        // Получение ссылки на первый элемент (неконстантная версия)
    const T& front() const;            // Получение ссылки на первый элемент (константная версия)
    bool empty() const;                // Проверка, пуста ли очередь
    size_t size() const;               // Получение текущего размера очереди

    // Дополнительные методы
    void clear();                      // Очистка всей очереди
    void display() const;              // Вывод содержимого очереди (для отладки)
};

// Явная инстанциация шаблона для распространенных типов
template class Queue<int>;
template class Queue<double>;


#endif // QUEUE_H