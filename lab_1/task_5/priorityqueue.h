#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <cstddef>
#include <stdexcept>

class PriorityQueue {
    struct Node {
        int key;
        char* value;
        Node(int k, const char* v);
        ~Node();
    };

    Node** heap;
    std::size_t capacity;
    std::size_t size;

    void resize();
    void heapifyUp(std::size_t index);
    void heapifyDown(std::size_t index);
    static std::size_t stringLength(const char* str);
    static char* stringCopy(const char* source);
    void swap(PriorityQueue& other) noexcept;

public:
    PriorityQueue();
    ~PriorityQueue();
    PriorityQueue(const PriorityQueue& other);
    PriorityQueue& operator=(PriorityQueue other);

    void add(int key, const char* value);
    const char* findMax() const;
    void removeMax();
    PriorityQueue& merge(const PriorityQueue& other);
    void clear();
    bool isEmpty() const;
};

#endif
