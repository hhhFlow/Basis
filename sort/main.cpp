#include <iostream>
// #include "bubble_sort.h"
#include "insert_sort.h"
#include "merge_sort.h"
#include "quick_sort.h"

int main() {
  int a[] = {10, 80, 30, 90, 40, 50, 70};

  quick_sort(a, sizeof(a) / sizeof(int));
  // merge_sort(a, sizeof(a) / sizeof(int));

  for (int i = 0; i < sizeof(a) / sizeof(int); ++i) {
    std::cout << " " << a[i];
  }
  std::cout << std::endl;
  return 0;
}