#include <stdio.h>
#include <stdlib.h>

typedef struct link {
    int data;
    struct link *next;
} node;

node* insert(int, node*);
node* delete(int, node*);
void printList(node*);
void collectGarbage(node*);

int main(int argc, char const *argv[]) {
    node *head = NULL;
    head = delete(900, head);
    head = insert(5, head);
    printList(head);
    head = delete(5, head);
    printList(head);
    head = delete(5, head);
    head = insert(7, head);
    head = insert(4, head);
    head = insert(3, head);
    head = insert(900, head);
    head = insert(0, head);
    head = insert(10, head);
    printList(head);
    head = delete(900, head);
    printList(head);
    collectGarbage(head);
    return 0;
}

node* insert(int data, node* head) {
    node* new = (node*) malloc(sizeof(node));
    new -> data = data;
    if (head == NULL || data < head -> data) {
        new -> next = head;
        head = new;
        return head;
    }
    node* current = head;
    while (current -> next != NULL && data > (current -> next -> data)) {
        current = current -> next;
    }
    new -> next = current -> next;
    current -> next = new;
    return head;
}

void printList(node* head) {
    while (head != NULL) {
        printf("%d -> ", head -> data);
        head = head -> next;
    }
    printf("NULL\n");
}

node* delete(int data, node* head) {
    node* temp = head, *current = head;
    if (temp == NULL) {
        printf("Error: empty list\n");
        return head;
    }
    if (temp -> data == data) {
        head = head -> next;
        free(temp);
        return head;
    }
    temp = temp -> next;
    while (temp -> next != NULL && data >= (temp -> next -> data)) {
        current = temp;
        temp = temp -> next;
    }
    if (data == temp -> data) {
        current -> next = temp -> next;
        free(temp);
    }
    return head;
}

void collectGarbage(node* head) {
    node* temp;
    for (temp = head; temp != NULL; temp = head) {
        head = temp -> next;
        free(temp);
    }
}
