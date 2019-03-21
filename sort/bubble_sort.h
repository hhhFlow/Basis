#define BUBBLE_SORT__H
#ifdef BUBBLE_SORT__H

template <typename T>
void swap(T &a, T &b) {
  // a = a^b;
  // b = a^b;
  // a = a^b;
  int temp = a;
  a = b;
  b = temp;
}

template <typename T>
void bubble_sort(T array[], int n) {
  int right = n - 1;
  for (int i = 1; i < n; ++i) {  //循环n-1遍。
    for (int j = 0; j < right; j++) {  //从头到最右端未有序位置，依次比较交换
      if (array[j] > array[j + 1]) swap(array[j], array[j + 1]);
    }
    right--;  //将最右端减1，因为最右端已经有序。
  }
}
#endif