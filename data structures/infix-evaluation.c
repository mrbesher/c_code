#include <stdlib.h>
#include <stdio.h>

typedef struct link {
    char data;
    struct link *next;
} node;

typedef struct numberlink {
    int operator;
    int data;
    struct numberlink *next;
} nnode;

void push(char, node**);
void pushn(int, nnode**, int);
char peek(node*);
char pop(node**);
nnode popn(nnode**);
void checkChar(char, node**);
int isOperator(char);
void processOperator(char, node**, nnode**);
int precedence(char);
void evaluate(char*);
void processPostfix(nnode**);

int main(int argc, char const *argv[]) {
    char text[100];
    fgets(text, 100, stdin);
    evaluate(text);
    // while (text[i] && text[i] != '\n') {
    //     checkChar(text[i], &top);
    //     i++;
    // }
    // while ((toPrint = pop(&top)) != -1) {
    //     printf("%c", toPrint);
    // };
    printf("\n");
    return 0;
}

void evaluate(char* text) {
    node* top = NULL;
    nnode* postfix = NULL;
    int i = 0, current, j;
    while (text[i] && text[i] != '\n') {
        if (isOperator(text[i])) {
            processOperator(character, &top, &postfix);
            i++;
        } else if (isNum(text[i])) {
            current = 0;
            j = 1;
            do {
                current = current * j + text[i] - '0';
                j *= 10;
                i++;
            } while(isNum(text[i]));
            pushn(current, &postfix, 0);
        } else {
            printf("Error: Can't process input. Please make sure that it is valid!\n");
            return;
        }
    }
    while ((current = pop(&top)) != -1) {
        pushn(current, &postfix, n);
    };
    processPostfix(postfix);
}

int isOperator(char character) {
    return character == '(' || character == ')' || character == '*' || character == '+' || character == '/' || character == '-' || character == '^';
}

int isNum(char character) {
    return '0' <= character && character <= '9';
}

void processOperator(char character, node** topAdd, nnode** postfix) {
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
            pushn(character, postfix, 1);
        }
        return;
    }
    while (precedence(peek(*topAdd)) >= precedence(character)) {
        pushn(pop(topAdd), postfix, 1);
    }
    push(character, topAdd);
}

void processPostfix(nnode** postfix) {
    nnode value;
    
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

void pushn(int number, nnode** topAdd, int operator) {
    nnode* new = (nnode*) malloc(sizeof(node));
    new -> next = *topAdd;
    new -> data = number;
    new -> operator = operator;
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

nnode popn(nnode** topAdd) {
    if (!*topAdd) {
        return -1;
    }
    char character = (*topAdd) -> data;
    node* temp = *topAdd;
    *topAdd = (*topAdd) -> next;
    free(temp);
    return character;
}
