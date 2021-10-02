#include <stdio.h>
#include <stdlib.h>

typedef struct link {
    int data;
    struct link *next;
} node;

void push(int, node**);
void peek(node*);
int pop(node**);

int main(int argc, char const *argv[]) {
    node *top = NULL;
    int number;
    peek(top);
    push(5, &top);
    push(100, &top);
    peek(top);
    printf("Got: %d\n", pop(&top));
    printf("Got: %d\n", pop(&top));
    printf("Got: %d\n", pop(&top));
    printf("Got: %d\n", pop(&top));
    return 0;
}

void push(int number, node** topAdd) {
    node* new = (node*) malloc(sizeof(node));
    new -> next = *topAdd;
    new -> data = number;
    *topAdd = new;
}

void peek(node* top) {
    if (!top) {
        printf("Error: empty stack\n");
        return;
    }
    printf("Top shows: %d\n", top -> data);
}

int pop(node** topAdd) {
    if (!*topAdd) {
        printf("Error: Stack Underflow\n");
        return -1;
    }
    int number = (*topAdd) -> data;
    node* temp = *topAdd;
    *topAdd = (*topAdd) -> next;
    free(temp);
    return number;
}
