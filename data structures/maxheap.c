#include <stdio.h>
#include <stdlib.h>

void swap(int*, int*);
void maxheapify(int*, int, int);
void insert(int*, int, int*);
void deleteInternal(int*, int, int*);

int main(int argc, char const *argv[]) {

    return 0;
}

void swap(int *x, int *y) {
    int temp = *y;
    *y = *x;
    *x = temp;
}

void maxheapify(int* array, int size, int i) {
    int greatest = i, left = 2*i+1, right = 2*i+2;
    if (size != 1) {
        if (left < size && array[left] > array[greatest]) {
            greatest = left;
        }
        if (right < size && array[right] > array[greatest]) {
            greatest = right;
        }
        if (greatest != i) {
            swap(&array[i], &array[greatest]);
            maxheapify(array, size, greatest);
        }
    }
}

void insert(int* array, int num, int* size) {
    int i;
    if ((*size) == 0) {
        array[0] = num;
        (*size)++;
    } else {
        array[(*size)] = num;
        (*size)++;
        for (i = (*size) / 2 - 1; i >= 0; i--) {
            maxheapify(array, (*size), i);
        }
    }
}

void deleteInternal(int* array, int num, int* size) {
    int i = 0;
    while (i < (*size) && num != array[i])
        i++;
    swap(&array[i], &array[(*size) -1]);
    (*size--);
    for (i = 0; i < count; i++)
        maxheapify(array, (*size), i);
}
