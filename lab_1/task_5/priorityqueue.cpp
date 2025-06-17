#include "priorityqueue.h"

std::size_t PriorityQueue::stringLength(const char* str) {
    if (!str) return 0;
    std::size_t len = 0;
    while (str[len]) ++len;
    return len;
}

char* PriorityQueue::stringCopy(const char* source) {
    if (!source) return nullptr;
    std::size_t len = stringLength(source);
    char* dest = new char[len + 1];
    for (std::size_t i = 0; i <= len; ++i) dest[i] = source[i];
    return dest;
}

PriorityQueue::Node::Node(int k, const char* v) : key(k) {
    if (!v) throw std::invalid_argument("Null value");
    value = stringCopy(v);
}

PriorityQueue::Node::~Node() {
    delete[] value;
}

PriorityQueue::PriorityQueue() : capacity(10), size(0) {
    heap = new Node*[capacity];
}

PriorityQueue::~PriorityQueue() {
    clear();
    delete[] heap;
}

PriorityQueue::PriorityQueue(const PriorityQueue& other)
    : capacity(other.capacity), size(other.size) {
    heap = new Node*[capacity];
    for (std::size_t i = 0; i < size; ++i) {
        heap[i] = new Node(other.heap[i]->key, other.heap[i]->value);
    }
}

PriorityQueue& PriorityQueue::operator=(PriorityQueue other) {
    swap(other);
    return *this;
}

void PriorityQueue::swap(PriorityQueue& other) noexcept {
    std::size_t tmpCap = capacity;
    capacity = other.capacity;
    other.capacity = tmpCap;
    std::size_t tmpSize = size;
    size = other.size;
    other.size = tmpSize;
    Node** tmpHeap = heap;
    heap = other.heap;
    other.heap = tmpHeap;
}

void PriorityQueue::add(int key, const char* value) {
    if (!value) throw std::invalid_argument("Null value");
    if (size == capacity) resize();
    heap[size] = new Node(key, value);
    heapifyUp(size++);
}

const char* PriorityQueue::findMax() const {
    if (isEmpty()) throw std::runtime_error("Queue is empty");
    return heap[0]->value;
}

void PriorityQueue::removeMax() {
    if (isEmpty()) throw std::runtime_error("Queue is empty");
    Node* removed = heap[0];
    heap[0] = heap[size - 1];
    --size;
    if (size > 0) heapifyDown(0);
    delete removed;
}

PriorityQueue& PriorityQueue::merge(const PriorityQueue& other) {
    for (std::size_t i = 0; i < other.size; ++i) {
        add(other.heap[i]->key, other.heap[i]->value);
    }
    return *this;
}

void PriorityQueue::clear() {
    for (std::size_t i = 0; i < size; ++i) {
        delete heap[i];
    }
    size = 0;
}

bool PriorityQueue::isEmpty() const {
    return size == 0;
}

void PriorityQueue::resize() {
    capacity *= 2;
    Node** newHeap = new Node*[capacity];
    for (std::size_t i = 0; i < size; ++i) {
        newHeap[i] = heap[i];
    }
    delete[] heap;
    heap = newHeap;
}

void PriorityQueue::heapifyUp(std::size_t index) {
    while (index > 0) {
        std::size_t parent = (index - 1) / 2;
        if (heap[index]->key <= heap[parent]->key) break;
        Node* tmp = heap[index];
        heap[index] = heap[parent];
        heap[parent] = tmp;
        index = parent;
    }
}

void PriorityQueue::heapifyDown(std::size_t index) {
    while (true) {
        std::size_t left = 2 * index + 1;
        std::size_t right = 2 * index + 2;
        std::size_t largest = index;
        if (left < size && heap[left]->key > heap[largest]->key) largest = left;
        if (right < size && heap[right]->key > heap[largest]->key) largest = right;
        if (largest == index) break;
        Node* tmp = heap[index];
        heap[index] = heap[largest];
        heap[largest] = tmp;
        index = largest;
    }
}
