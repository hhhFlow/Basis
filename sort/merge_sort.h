

template <typename T>
void merge(T array[], int left, int mid, int right) {
  int start = left;

  int n1 = mid - left + 1;
  int n2 = right - mid;

  T *l = new int[n1];
  T *r = new int[n2];

  for (int i = 0; i < n1; ++i) l[i] = array[left++];
  for (int i = 0; i < n2; ++i) r[i] = array[++mid];

  int i = 0, j = 0;
  while ((i < n1) && (j < n2)) {
    if (l[i] < r[j])
      array[start++] = l[i++];
    else
      array[start++] = r[j++];
  }

  while (i < n1) {
    array[start++] = l[i++];
  }

  while (j < n2) {
    array[start++] = r[j++];
  }
}

template <typename T>
void merge_sort(T array[], int left, int right) {
  if (left < right) {
    int mid = (left + right) / 2;
    merge_sort(array, left, mid);
    merge_sort(array, mid + 1, right);
    merge(array, left, mid, right);
  }
}

template <typename T>
void merge_sort(T array[], int n) {
  int left = 0, right = n - 1;
  merge_sort(array, left, right);
}