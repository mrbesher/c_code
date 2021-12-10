#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comparInt(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

void printArr(int *arr, size_t nmemb) {
    for (size_t i = 0; i < nmemb; i++) {
        printf("%d\t", arr[i]);
    }
    printf("\n");
}

void swap(void *a, void *b, size_t size) {
    void *temp = malloc(size);
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
}

size_t partition(void *base, size_t nmemb, size_t size,
                 int (*compar)(const void *, const void *)) {
    void *pivot = base + (nmemb - 1) * size;  // last element as a pivot
    int i = -1;                               // position the pivot should be in
    size_t j;                                 // loop variable

    for (j = 0; j < nmemb - 1; j++) {
        // if compar says pivot is greater swap
        if (compar(base + j * size, pivot) < 0)
            i++, swap(base + i * size, base + j * size, size);
    }
    swap(base + (++i) * size, pivot, size);
    return (size_t)i;
}

void myqsort(void *base, size_t nmemb, size_t size,
             int (*compar)(const void *, const void *)) {
    if (nmemb < 2)
        return;

    size_t offset = partition(base, nmemb, size, compar);
    myqsort(base, offset, size, compar);
    myqsort(base + (offset + 1) * size, nmemb - (offset + 1), size, compar);
}

int main() {
    int arr[10] = {10, 4, 1, 11, 3, 6, 1, 19, 0, 12};
    myqsort(arr, 10, sizeof(int), comparInt);
    printArr(arr, 10);
    return 0;
}
