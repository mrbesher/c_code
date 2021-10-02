#include <stdio.h>
#include <stdlib.h>

typedef struct link {
    char data;
    struct link *next;
} node;

void push(char, node**);
void peek(node*);
char pop(node**);
int checkChar(char, node**);

int main(int argc, char const *argv[]) {
    char text[100];
    int i=0;
    node* top = NULL;
    fgets(text, 100, stdin);
    while (text[i] && checkChar(text[i], &top)) {i++;}
    if (text[i]) {
        printf("Bad char at %d\n", i);
    } else if ((i = pop(&top)) != -1) {
        printf("parantheses not closed '%c'\n", i);
    }
    while (pop(&top) != -1);
    return 0;
}
int checkChar(char toCheck, node** topAdd) {
    static char ascii[128];
    ascii['('] = ascii[')'] = '(';
    ascii['['] = ascii[']'] = '[';
    ascii['{'] = ascii['}'] = '{';
    if (ascii[(int)toCheck] == toCheck) {
        push(toCheck, topAdd);
        return -1;
    }
    if (!ascii[(int)toCheck]) {
        return -1;
    }
    return pop(topAdd) == ascii[(int)toCheck];
}

void push(char character, node** topAdd) {
    node* new = (node*) malloc(sizeof(node));
    new -> next = *topAdd;
    new -> data = character;
    *topAdd = new;
}

void peek(node* top) {
    if (!top) {
        printf("Error: empty stack\n");
        return;
    }
    printf("Top shows: %c\n", top -> data);
}

char pop(node** topAdd) {
    if (!*topAdd) {
        return -1;
    }
    char character = (*topAdd) -> data;
    node* temp = *topAdd;
    *topAdd = (*topAdd) -> next;
    free(temp);
    return character;
}
