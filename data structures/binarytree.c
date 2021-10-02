#include <stdio.h>
#include <stdlib.h>

typedef struct vertex {
    int data;
    struct vertex *right;
    struct vertex *left;
} node;

void insert(int, node**);
void destroy(node**);
void display(node**);

int main(int argc, char const *argv[]) {
    node *root = NULL;
    insert(8, &root);
    insert(5, &root);
    insert(9, &root);
    insert(6, &root);
    insert(7, &root);
    display(&root);
    printf("\n");
    destroy(&root);
    return 0;
}

void insert(int number, node** verAdd) {
    /*
    Inserts elements to the tree
    If the inserted element is greater than the current one then it is insersted to the right
    otherwise it is inserted to the left
    */
    if (!*verAdd) {
        *verAdd = (node*) malloc(sizeof(node));
        (*verAdd) -> right = (*verAdd) -> left = NULL;
        (*verAdd) -> data = number;
        return;
    }
    if (number > ((*verAdd) -> data)) {
        insert(number, &((*verAdd) -> right));
    } else {
        insert(number, &((*verAdd) -> left));
    }
}

void destroy(node** verAdd) {
    /*
    frees every vertex in the tree following a depth-first technique
    */
    if ((*verAdd) -> left) {
        destroy(&((*verAdd) -> left));
    }
    if ((*verAdd) -> right) {
        destroy(&((*verAdd) -> right));
    }
    free(*verAdd);
}

void display(node** verAdd) {
    /*
    Displays tree elements traversing in-order
    */
    if ((*verAdd) -> left) {
        display(&((*verAdd) -> left));
    }
    printf("%d\t", (*verAdd) -> data);
    if ((*verAdd) -> right) {
        display(&((*verAdd) -> right));
    }
}
