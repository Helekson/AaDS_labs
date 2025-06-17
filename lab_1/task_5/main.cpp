#include "priorityqueue.h"
#include <iostream>

bool stringsEqual(const char* a, const char* b) {
    if (a == b) return true;
    if (!a || !b) return false;
    while (*a && *b && *a == *b) ++a, ++b;
    return *a == *b;
}

void testBasicOperations() {
    PriorityQueue pq;
    pq.add(3, "Apple");
    pq.add(5, "Banana");
    pq.add(1, "Cherry");
    if (!stringsEqual(pq.findMax(), "Banana")) std::cerr << "Test 1 failed\n";
    pq.removeMax();
    if (!stringsEqual(pq.findMax(), "Apple")) std::cerr << "Test 2 failed\n";
}

void testMerge() {
    PriorityQueue pq1, pq2;
    pq1.add(2, "Mango");
    pq2.add(5, "Pineapple");
    pq1.merge(pq2);
    if (!stringsEqual(pq1.findMax(), "Pineapple")) std::cerr << "Test 3 failed\n";
}

void testExceptions() {
    PriorityQueue pq;
    try { pq.findMax(); std::cerr << "Test 4 failed\n"; }
    catch (...) {}
    try { pq.add(1, nullptr); std::cerr << "Test 5 failed\n"; }
    catch (...) {}
}

int main() {
    testBasicOperations();
    testMerge();
    testExceptions();
    std::cout << "All tests executed.\n";
    return 0;
}
