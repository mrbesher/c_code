#include <stdio.h>
#include <stdlib.h>

typedef struct link {
    int data;
    struct link *next;
} node;

void enqueue(int, node**, node**);
int dequeue(node**, node**);

int main(int argc, char const *argv[]) {
    node *front = NULL;
    node *rear = NULL;
    enqueue(5, &front, &rear);
    enqueue(15, &front, &rear);
    printf("Got: %d\n", dequeue(&front, &rear));
    enqueue(20, &front, &rear);
    enqueue(3, &front, &rear);
    printf("Got: %d\n", dequeue(&front, &rear));
    printf("Got: %d\n", dequeue(&front, &rear));
    enqueue(55, &front, &rear);
    printf("Got: %d\n", dequeue(&front, &rear));
    printf("Got: %d\n", dequeue(&front, &rear));
    printf("Got: %d\n", dequeue(&front, &rear));
    printf("Got: %d\n", dequeue(&front, &rear));
    return 0;
}

void enqueue(int number, node** frontAdd, node** rearAdd) {
    node* new = (node*) malloc(sizeof(node));
    new -> next = NULL;
    new -> data = number;
    if (*rearAdd) {
        (*rearAdd) -> next = new;
    }
    *rearAdd = new;
    if (!*frontAdd) {
        *frontAdd = *rearAdd;
    }
}

int dequeue(node** frontAdd, node** rearAdd) {
    if (!*frontAdd) {
        printf("Error: Empty Queue\n");
        return -1;
    }
    int number = (*frontAdd) -> data;
    node* temp = *frontAdd;
    *frontAdd = (*frontAdd) -> next;
    if (!*frontAdd) {
        *rearAdd = NULL;
    }
    free(temp);
    return number;
}
