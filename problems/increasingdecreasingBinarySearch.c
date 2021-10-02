#include <stdio.h>
#include <stdlib.h>

int findMaximum(int *arr, int low, int high) {
  int mid;
   if (low == high)
     return low;

   if ((high == low + 1) && arr[low] >= arr[high])
      return low;

   if ((high == low + 1) && arr[low] < arr[high])
      return high;

   mid = (int)((low + high)/2);

   if ( arr[mid] > arr[mid + 1] && arr[mid] > arr[mid - 1])
      return mid;

   if (arr[mid] > arr[mid + 1] && arr[mid] < arr[mid - 1])
     return findMaximum(arr, low, mid-1);

  return findMaximum(arr, mid + 1, high);
}

int binarySearch(int key, int *arr, int low, int high) {
  int mid;
  if (low > high)
    return -1;

  mid = (int)((low + high) / 2);

  if (arr[mid] == key)
    return mid;

  if (arr[mid] > key)
    return binarySearch(key, arr, mid+1, high);

  return binarySearch(key, arr, low, mid-1);
}

int semiBinary(int key, int *arr, int n) {
  int result, max;
  max = findMaximum(arr, 0, n-1);
  if ((result = binarySearch(key, arr, 0, max)) == -1) {
    return binarySearch(key, arr, max+1, n-1);
  }
  return result;
}

int main(int argc, char const *argv[]) {
  int key, n, i, *arr;
  printf("Enter n: "); scanf(" %d", &n);
  arr = (int*) malloc(n * sizeof(int));
  for (i = 0; i < n; i++){
    printf("[%d]: ", i); scanf(" %d", &arr[i]);
  }
  printf("Element to search: "); scanf(" %d", &key);
  printf("RES: %d\n", semiBinary(key, arr, n));
  free(arr);
  return 0;
}
