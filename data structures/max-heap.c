#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
  int temp = *b;
  *b = *a;
  *a = temp;
}

void maxheapify(int array[], int size, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (size != 1) {
        if (l < size && array[l] > array[largest])
          largest = l;
        if (r < size && array[r] > array[largest])
          largest = r;
        if (largest != i)
        {
          swap(&array[i], &array[largest]);
          maxheapify(array, size, largest);
        }
    }
}

void insert(int* array, int newNum, int* size)
{
    int i;
    if ((*size) == 0) {
        array[0] = newNum;
        (*size)++;
    } else {
        array[(*size)] = newNum;
        (*size)++;
        for (i = (*size) / 2 - 1; i >= 0; i--) {
            maxheapify(array, (*size), i);
        }
    }
}

void deleteRoot(int* array, int num, int* size)
{
    int i=0;
    while (i < (*size) && num != array[i]) {
        i++
    }
    swap(&array[i], &array[(*size) - 1]);
    (*size)--;
    for (i = (*size) / 2 - 1; i >= 0; i--) {
        maxheapify(array, (*size), i);
    }
}

void printArray(int* array, int size)
{
    for (int i = 0; i < size; ++i)
        printf("%d ", array[i]);
    printf("\n");
}

int main()
{
    int size, *array, heapsize = 0;
    printf("Enter array size: ");
    scanf(" %d", &size);
    array = (int*) malloc(size * sizeof(int));
    insert(array, 3, &heapsize);
    insert(array, 4, &heapsize);
    insert(array, 9, &heapsize);
    insert(array, 5, &heapsize);
    insert(array, 2, &heapsize);
    printf("Max-Heap array: ");
    printArray(array, size);
    deleteRoot(array, 4, &heapsize);
    printf("After deleting an element: ");
    printArray(array, size);
}
