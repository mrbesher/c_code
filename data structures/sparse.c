#include <stdio.h>
#include <stdlib.h>

typedef struct link {
    int data;
    int row;
    int column;
    struct link *next;
} node;

void addInput(node**, int, int, int);
void printZeroColumns(node*, int);
void destroyAll(node**);

int main(int argc, char const *argv[]) {
    int rows, columns, i, j, num;
    node *front = NULL;
    printf("Enter rows number: ");
    scanf(" %d", &rows);
    printf("Enter columns number: ");
    scanf(" %d", &columns);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++) {
            printf("[%d,%d]: ", i+1, j+1);
            scanf(" %d", &num);
            addInput(&front, num, i, j);
        }
    }
    printZeroColumns(front, columns);
    //destroyAll(&front);
    return 0;
}

void addInput(node** topAdd, int num, int i, int j) {
    if (!num) {
        return;
    }
    node* new = (node*) malloc(sizeof(node));
    new -> next = *topAdd;
    new -> data = num;
    new -> row = i;
    new -> column = j;
    *topAdd = new;
}

void printZeroColumns(node* front, int columns) {
    int* columnNums = (int*)malloc(columns*sizeof(int));
    int i;
    node* temp;
    for (i = 0; i < columns; i++)
        columnNums[i] = 1;
    for (temp = front; temp -> next != NULL; temp = temp -> next) {
        columnNums[temp -> column] = 0;
    }
    for (i = 0; i < columns; i++) {
        if (columnNums[i]) {
            printf("column %d is filled with 0s\n", i);
        }
    }
    free(columnNums);
}

void destroyAll(node** topAdd) {
    node* temp;
    for (temp = (*topAdd) -> next; temp != NULL; (*topAdd) = (*topAdd) -> next, temp = (*topAdd)) {
        free(*topAdd);
    }
}
