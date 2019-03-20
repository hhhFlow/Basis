#include <iostream>
#include "bubble_sort.h"
#include "insert_sort.h"
#include "merge_sort.h"

int main() {
  int a[] = {654, 17, 39, 21, 5, 1, 2, 8, 99, 1, 45, 34, 66};

  merge_sort(a, sizeof(a) / sizeof(int));

  for (int i = 0; i < sizeof(a) / sizeof(int); ++i) {
    std::cout << " " << a[i];
  }
  std::cout << std::endl;
  return 0;
}