//
// Created by atman on 12/15/25.
//
#include <iostream>
#include <vector>

using std::cout, std::string, std::vector;

template<typename T>
class Heap {
public:
    explicit Heap(vector<T> data) {
        heap_data = data;
    }

private:
    vector<T> heap_data;
    int parent(int index) {
        return (index - 1) / 2;
    }
    int left(int index){}

    static void heapify() {

    }
};
