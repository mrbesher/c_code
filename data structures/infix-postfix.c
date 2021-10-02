#include <stdlib.h>
#include <stdio.h>

typedef struct link {
    char data;
    struct link *next;
} node;

void push(char, node**);
char peek(node*);
char pop(node**);
void checkChar(char, node**);
int isOperator(char);
void processOperator(char, node**);
int precedence(char);

int main(int argc, char const *argv[]) {
    char text[100], toPrint;
    int i=0;
    node* top = NULL;
    fgets(text, 100, stdin);
    while (text[i] && text[i] != '\n') {
        checkChar(text[i], &top);
        i++;
    }
    while ((toPrint = pop(&top)) != -1) {
        printf("%c", toPrint);
    };
    printf("\n");
    return 0;
}

void checkChar(char character, node** topAdd) {
    isOperator(character) ? processOperator(character, topAdd) : printf("%c", character);
}

int isOperator(char character) {
    return character == '(' || character == ')' || character == '*' || character == '+' || character == '/' || character == '-' || character == '^';
}

void processOperator(char character, node** topAdd) {
    if (peek(*topAdd) == -1) {
        push(character, topAdd);
        return;
    }
    if (character == '(') {
        push(character, topAdd);
        return;
    }
    if (character == ')') {
        while ((character = pop(topAdd)) != '(') {
            printf("%c", character);
        }
        return;
    }
    while (precedence(peek(*topAdd)) >= precedence(character)) {
        printf("%c", pop(topAdd));
    }
    push(character, topAdd);
}

int precedence(char operator) {
    if (operator == '^') {
        return 5;
    }
    if (operator == '*' || operator == '/') {
        return 4;
    }
    if (operator == '+' || operator == '-') {
        return 3;
    }
    return 0;
}

void push(char character, node** topAdd) {
    node* new = (node*) malloc(sizeof(node));
    new -> next = *topAdd;
    new -> data = character;
    *topAdd = new;
}

char peek(node* top) {
    if (!top) {
        return -1;
    }
    return top -> data;
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
