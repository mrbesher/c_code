#ifdef _WIN32
#include <windows.h> //import windows.h to change console color
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define RESET 0
#define CHAR_LIMIT 100

typedef struct vertex {
    char name[CHAR_LIMIT];
    struct vertex *parent;
    struct vertex *sibling;
    struct vertex *child;
} node;

typedef struct link {
    struct vertex *dir;
    struct link *prev;
    struct link *next;
} element;

void setColor(int);
void initializeEnv(node**, element**, element**);
int processInput(node**, element*, element**);
void printDir(element*);
void destroyAll(node**, element**, element**);
void destroyTree(node**);
int mkdir(node**,char*);
int chdir(element**, element*, char*);
int rmdir(node**, char*);
void dir(node**);
void count(node**);

int main(int argc, char const *argv[]) {
    element *start = NULL, *end = NULL;
    node *root = NULL;
    initializeEnv(&root, &start, &end);
    while (processInput(&root, start, &end));
    destroyAll(&root, &start, &end);
    return 0;
}

void initializeEnv(node** verAdd, element** startAdd, element** endAdd) {
    if (*verAdd) {
        return;
    }
    *verAdd = (node*) malloc(sizeof(node));
    (*verAdd) -> parent = (*verAdd) -> child = (*verAdd) -> sibling = NULL;
    strcpy((*verAdd) -> name, "~");
    *startAdd = (element*) malloc(sizeof(element));
    (*startAdd) -> dir = *verAdd;
    (*startAdd) -> prev = (*startAdd) -> next = NULL;
    *endAdd = *startAdd;
}

int processInput(node** verAdd, element* startAdd, element** endAdd) {
    char *buffer, *temp, *arg, *cmd;
    int shouldCont = 1;
    buffer = (char*)malloc(CHAR_LIMIT*sizeof(char));
    cmd = (char*)malloc(CHAR_LIMIT*sizeof(char));
    arg = (char*)malloc(CHAR_LIMIT*sizeof(char));
    printDir(startAdd);
    //seperate arguments
    fgets(buffer, CHAR_LIMIT, stdin);
    temp = strtok(buffer, " ");
    strcpy(cmd, temp);
    temp = strtok(NULL, "\n");
    strtok(cmd, "\n");
    if (temp) {
        strcpy(arg, temp);
    }
    if (!strcmp(cmd, "mkdir")) {
        mkdir(&((*endAdd) -> dir), arg);
    } else if (!strcmp(cmd, "chdir")) {
        chdir(endAdd, startAdd, arg);
    } else if (!strcmp(cmd, "rmdir")) {
        rmdir(&((*endAdd) -> dir), arg);
    } else if (!strcmp(cmd, "dir")) {
        dir(&((*endAdd) -> dir));
    } else if (!strcmp(cmd, "count")) {
        count(&((*endAdd) -> dir));
    } else if (!strcmp(cmd, "exit")) {
        shouldCont = 0;
    }
    free(buffer),free(arg),free(cmd);
    return shouldCont;
}

void dir(node** verAdd) {
    node* temp = (*verAdd) -> child;
    if (!temp) {
        return;
    }
    while (temp -> sibling) {
        printf("%s\t", temp -> name);
        temp = temp -> sibling;
    }
    printf("%s",temp->name);
    printf("\n");
}

int mkdir(node** verAdd, char* name) {
    if (!((*verAdd) -> child)) {
        (*verAdd) -> child = (node*) malloc(sizeof(node));
        (*verAdd) -> child -> parent = *verAdd;
        (*verAdd) -> child -> sibling = (*verAdd) -> child -> child = NULL;
        strcpy((*verAdd) -> child -> name, name);
        return 1;
    }
    node* temp = (*verAdd) -> child;
    while (strcmp(name, temp -> name) && temp -> sibling) {
        temp = temp -> sibling;
    }
    if (!strcmp(name, temp -> name)) {
        setColor(RED);
        printf("Error: Duplicate directory name\n");
        setColor(RESET);
        return 0;
    }
    temp -> sibling = (node*) malloc(sizeof(node));
    temp -> sibling -> parent = *verAdd;
    temp -> sibling -> sibling = temp -> sibling -> child = NULL;
    strcpy(temp -> sibling -> name, name);
    return 1;
}

int chdir(element** endAdd, element* startAdd, char* name) {
    if (!strcmp(name, "..") && (startAdd -> dir != (*endAdd) -> dir)) {
        element* tmpLink = (*endAdd);
        (*endAdd) -> prev -> next = NULL;
        (*endAdd) = (*endAdd) -> prev;
        free(tmpLink);
        return 1;
    } else if (!strcmp(name, "..") && startAdd -> dir == (*endAdd) -> dir) {
        setColor(RED);
        printf("Error: Already in root\n");
        setColor(RESET);
        return 0;
    }
    node* temp = (*endAdd) -> dir -> child;
    if (!temp) {
        setColor(RED);
        printf("Error: %s does not exist\n", name);
        setColor(RESET);
        return 0;
    }
    while (strcmp(temp -> name, name) && temp -> sibling) {
        temp = temp -> sibling;
    }
    if (!strcmp(temp -> name, name)) {
        (*endAdd) -> next = (element*) malloc(sizeof(element));
        (*endAdd) -> next -> dir = temp;
        (*endAdd) -> next -> prev = (*endAdd);
        (*endAdd) -> next -> next = NULL;
        (*endAdd) = (*endAdd) -> next;
        return 1;
    }
    setColor(RED);
    printf("Error: %s does not exist\n", name);
    setColor(RESET);
    return 0;
}

void count(node** verAdd) {
    node* temp = (*verAdd) -> child;
    int count = 0;
    if (!temp) {
        printf("%d\n",count);
        return;
    }
    count++;
    while (temp -> sibling) {
        temp = temp -> sibling;
        count++;
    }
    printf("%d\n", count);
}

void printDir(element* startAdd) {
    setColor(GREEN);
    printf("user@directorymaze");
    setColor(RESET);
    printf(":");
    setColor(BLUE);
    while (startAdd -> next) {
        printf("%s/", startAdd -> dir -> name);
        startAdd = startAdd -> next;
    }
    printf("%s", startAdd -> dir -> name);
    setColor(RESET);
    printf("$ ");
}

int rmdir(node** verAdd, char* name) {
    node *temp, *prev;
    if (!((*verAdd) -> child)) {
        setColor(RED);
        printf("Error: %s does not exist\n", name);
        setColor(RESET);
        return 0;
    } else if (!strcmp((*verAdd) -> child -> name, name)) {
        if ((*verAdd) -> child -> child) {
            setColor(RED);
            printf("Error: %s not empty\n", name);
            setColor(RESET);
            return 0;
        }
        temp = (*verAdd) -> child;
        (*verAdd) -> child = (*verAdd) -> child -> sibling;
        free(temp);
        return 1;
    }
    temp = (*verAdd) -> child;
    while (strcmp(name, temp -> name) && temp -> sibling) {
        prev = temp;
        temp = temp -> sibling;
    }
    if (!strcmp(name, temp -> name)) {
        prev -> sibling = temp -> sibling;
        free(temp);
        return 1;
    }
    setColor(RED);
    printf("Error: %s does not exist\n", name);
    setColor(RESET);
    return 0;
}

void destroyAll(node** verAdd, element** startAdd, element** endAdd) {
    destroyTree(verAdd);
    element *temp;
    while ((temp = (*startAdd))) {
        (*startAdd) = (*startAdd) -> next;
        free(temp);
    }
}

void destroyTree(node** verAdd) {
    /*
    frees every vertex in the tree following a depth-first technique
    */
    if ((*verAdd) -> child) {
        destroyTree(&((*verAdd) -> child));
    }
    if ((*verAdd) -> sibling) {
        destroyTree(&((*verAdd) -> sibling));
    }
    free(*verAdd);
}

//functions that are defined differently based on platform
#ifdef _WIN32
void setColor(int color) {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  switch (color) {
    case 31:
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    break;
    case 32:
      SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    break;
    case 33:
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    break;
    case 34:
      SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    break;
    default:
      SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
  }
}
#else
void setColor(int color) {
  printf("\x1b[%dm", color);
}
#endif
