#include <ios>
#include <iostream>
#include <vector>
#include <random>

void quick_sort(std::vector<int>& array,
                const int lower,
                const int upper) {
    if (lower >= upper)
        return;
    const int pivot = array[lower];
    int rp = upper;
    int lp = lower;
    while (lp < rp) {
        while (lp < rp && array[rp] >= pivot)
            rp --;
        if (lp < rp)
            array[lp] = array[rp];
        while (lp < rp && array[lp] <= pivot)
            lp ++;
        if (lp < rp)
            array[rp] = array[lp];
    }
    array[lp] = pivot;
    quick_sort(array, lower, lp - 1);
    quick_sort(array, lp + 1, upper);
}

void bubble_sort(std::vector<int>& array) {
    for (int i = 1; i < array.size(); i++) {
        bool flag = false;
        for (int j = 1; j < array.size() - i + 1; j ++) {
            if (array[j] < array[j - 1]){
                int tmp = array[j];
                array[j] = array[j-1];
                array[j - 1] = tmp;
                flag = true;
            }
        }
        if (!flag) break;
    }
}

void insert_sort(std::vector<int>& array) {
    for (int i = 1; i < array.size(); i++) {
        int left = 0;
        int right = i;
        while (left < right){
            int mid = left + (right - left)/2;
            if (array[mid] <= array[i])
                left = mid + 1;
            else
                right = mid;
        }
        for (int j = i; j > left;j --) {
            int tmp = array[j];
            array[j] = array[j-1];
            array[j-1] = tmp;
        }
    }
}

void shell_sort(std::vector<int>& array) {
    int gap = array.size() /2;
    while(gap) {
        for (int i = gap; i < array.size(); i++) {
            for (int j = i; j>0; j-= gap) {
                if (array[j-gap] > array[j]) {
                    int tmp = array[j-gap];
                    array[j-gap] = array[j];
                    array[j] = tmp;
                }
            }
        }
        gap /= 2;
    }
}

void sift_down(std::vector<int>& array, const int node) {
    int min = node;
    int left = node * 2 + 1;
    int right = node * 2 + 2;
    if (left < array.size() && array[left] < array[min])
        min = left;
    if (right < array.size() && array[right] < array[min])
        min = right;
    if (min != node) {
        int tmp = array[min];
        array[min] = array[node];
        array[node] = tmp;
        sift_down(array, min);
    }
    return;
}
void pop(std::vector<int>& array) {
    std::swap(array[0], array[array.size()]);
    array.pop_back();
    sift_down(array, 0);
}

void heap_sort(std::vector<int>& array) {
    for (int i = array.size() / 2; i >= 0; i--) {
        sift_down(array, i);
    }
    int n = array.size();
    for (int i = 0; i < n; i ++) {
        std::cout << array[0]<< " ";
        pop(array);
    }
}

int main() {
    std::vector<int> arr = {344,30, 3157, 1509, 8603, 1048,3,85,580,554};
    for (const auto& i:arr)
        std::cout << i << " ";
    std::cout << std::endl;
    heap_sort(arr);
    std::cout << std::endl;
}
