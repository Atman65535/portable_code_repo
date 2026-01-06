#include <iostream>
#include <vector>

void search_insert(std::vector<int>& array) {
  int n = array.size();
  for (int i = 1; i < n; i++) {
    int left = 0;
    int right = i;
    while (left < right) {
      int mid = left + (right - left)/2;
      if (array[mid] <= array[i]) {
        left = mid + 1;
      }
      else {
        right = mid;
      }
    }
    for (int j = i-1; j >= left; j--) {
      if (array[j] > array[j + 1]){
        std::swap(array[j], array[j+1]);
      }
      else break;
    }
  }
}
int main() {
  std::vector<int> arr = {2,3, 5, 1, 3, 12, 2, 32, 0};
  search_insert(arr);
  for (int i = 0; i< arr.size(); i++)
    std::cout << arr[i] << " ";
}
