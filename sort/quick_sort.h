#define QUICK_SORT__H
#ifdef QUICK_SORT__H

#include <iostream>
template <typename T>
void swap(T &a, T &b) {
  // a = a ^ b;
  // b = a ^ b;
  // a = a ^ b;
  int temp = a;
  a = b;
  b = temp;
}

template <typename T>
int partition(T array[], int left, int right) {
  int p = array[left];
  int i = left;
  for (int j = left + 1; j <= right; j++) {
    if (array[j] < p) swap(array[j], array[++i]);
  }
  swap(array[i], array[left]);
  return i;
}

template <typename T>
void quick_sort(T array[], int left, int right) {
  if (left < right) {
    int pivot = partition(array, left, right);
    quick_sort(array, left, pivot - 1);
    quick_sort(array, pivot + 1, right);
  }
}

template <typename T>
void quick_sort(T array[], int n) {
  quick_sort(array, 0, n - 1);
}

#endif