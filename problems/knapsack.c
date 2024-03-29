#include <stdio.h>
#include <stdlib.h>

typedef struct {
    /* can be added to make the game more fun */
    /* char name[20]; */
    int value;
    int weight;
} Item;

int get_max_weight() {
    int maxWeight;
    printf("How many weight units can you lift? ");
    scanf(" %d", &maxWeight);
    return maxWeight;
}

Item* get_items_from_user(size_t* size) {
    size_t i;
    Item* itemsArr;
    printf("How many items do you see? ");
    scanf(" %lu", size);
    itemsArr = (Item*)malloc((*size + 1) * sizeof(Item));
    /* Failed to allocate memory */
    if (!itemsArr)
        return itemsArr;

    for (i = 0; i < *size; i++) {
        printf("Value of item #%lu: ", i + 1);
        scanf(" %d", &itemsArr[i].value);
        printf("Weight of item #%lu: ", i + 1);
        scanf(" %d", &itemsArr[i].weight);
    }

    return itemsArr;
}

int** init_matrix(size_t size, int maxWeight) {
    size_t i;
    int** matrix = (int**)malloc((size + 1) * sizeof(int*));

    for (i = 0; i < size + 1; i++)
        matrix[i] = (int*)malloc((maxWeight + 1) * sizeof(int));

    for (i = 0; i < (size_t)maxWeight + 1; i++)
        matrix[0][i] = 0;

    for (i = 1; i < size + 1; i++)
        matrix[i][0] = 0;

    return matrix;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void print_taken_items(int** matrix, Item* itemsArr, size_t size, int maxWeight) {
    size_t i, currWeight;
    currWeight = maxWeight;
    for (i = 0; i < size; i++) {
        if (matrix[size - i][currWeight] != matrix[size - i - 1][currWeight]) {
            printf("Get the item #%lu (v:%d, w:%d)\n", size - i, itemsArr[size - i - 1].value, itemsArr[size - i - 1].weight);
            currWeight -= itemsArr[size - i - 1].weight;
        }
    }
}

int main() {
    size_t size, i, j;
    Item* itemsArr;
    int **matrix, maxWeight;

    printf("Hi thief! Let me help you with your adventure.\n");
    maxWeight = get_max_weight();
    itemsArr = get_items_from_user(&size);

    matrix = init_matrix(size, maxWeight);
    for (i = 1; i < size + 1; i++) {
        for (j = 1; j < (size_t)maxWeight + 1; j++) {
            if (itemsArr[i - 1].weight > (int)j) {
                matrix[i][j] = matrix[i - 1][j];
            } else {
                matrix[i][j] = max(itemsArr[i - 1].value + matrix[i - 1][j - itemsArr[i - 1].weight], matrix[i - 1][j]);
            }
        }
    }

    printf("Max. value possible: %d. Hurray!\n", matrix[size][maxWeight]);
    print_taken_items(matrix, itemsArr, size, maxWeight);
    printf("This is the optimal solution, turst me.\nHurry up!\n");

    for (i = 0; i < size + 1; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(itemsArr);
    return 0;
}
