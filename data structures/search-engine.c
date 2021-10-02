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
#define MAX_RECOM 3

typedef struct query query;
typedef struct Graph Graph;
typedef struct node node;


struct query {
  int id;
  char name[CHAR_LIMIT];
  int searchNumber;
};

struct node {
  int vertex;
  query* data;
  node* next;
};

struct Graph {
  int numVertices;
  node** adjLists;
  query** queries;
};

// utilites
void setColor(int);
int getNumber();
char* scanRaw(int);

//graph functions
Graph* createGraph(int);
void addEdge(Graph*, int, int);
node* createNode(Graph*, int);

// program specific functions
query* createQuery(int, char*, int);
void printQueryList(query**, int);
Graph* initializeGraph();
void greet();
void handleRelationsString(Graph*, int, char*);
void parseRelation(char*, char*, int*, int*);
Graph* mergeGraphs(Graph*, Graph*);
int findInGraph(Graph*, char*);
void mergeAdjLists(node**, node**, int*, Graph*);
void freeAdjList(node*);
int searchEngine(Graph*);
void printLorem(char*);
void printRecommendations(Graph*, char*);
void freeGraph(Graph*);
void printGraph(Graph*);

int main(int argc, char const *argv[]) {
  Graph *graph1, *graph2, *mainGraph;
  printf("Welcome to Inquisitio. Let's get you set up!\nFor the 1st Graph:\n");
  graph1 = initializeGraph();
  printf("For the 2nd graph:\n");
  graph2 = initializeGraph();
  mainGraph = mergeGraphs(graph1, graph2);
  printf("The resulting graph is as follows:\n");
  printGraph(mainGraph);
  while (searchEngine(mainGraph));
  freeGraph(graph1);
  freeGraph(graph2);
  freeGraph(mainGraph);
  return 0;
}

void greet() {
  printf("Explore the World Wide Web!\n");
  setColor(RED);
  printf(" (_)_ __   __ _ _   _(_)___(_) |_(_) ___  \n");
  setColor(YELLOW);
  printf(" | | '_ \\ / _` | | | | / __| | __| |/ _ \\ \n");
  setColor(RESET);
  printf(" | | | | | (_| | |_| | \\__ \\ | |_| | (_) |\n");
  setColor(GREEN);
  printf(" |_|_| |_|\\__, |\\__,_|_|___/_|\\__|_|\\___/ \n");
  setColor(BLUE);
  printf("             |_|                          \n");
  setColor(YELLOW);
  printf("Type 'exit' to quit\n");
  setColor(RESET);
  printf("All rights reserved 1985-1990.\n");
}

void printGraph(Graph* graph) {
  int v;
  for (v = 0; v < graph->numVertices; v++) {
    struct node* temp = graph->adjLists[v];
    printf("['%s']: ", graph -> queries[v] -> name);
    printf("(");
    while (temp) {
      printf("%s", temp->data->name);
      if (temp -> next) {
        printf(" - ");
      }
      temp = temp->next;
    }
    printf(")\n");
  }
  printf("\n");
}

void freeGraph(Graph* graph) {
  int i;
  free(graph -> adjLists);
  for (i = 0; i < graph -> numVertices; i++) {
    if (graph -> queries[i]) {
      free(graph -> queries[i]);
    }
  }
  free(graph -> queries);
  free(graph);
}

void printRecommendations(Graph* graph, char* input) {
  setColor(YELLOW);
  int i, j, maxQueries=0, toPrint=0, loc;
  printf("\n");
  if ((loc = findInGraph(graph, input)) == -1) {
    printf("That's a very unique query! Haven't seen people searching this here before.\n\n");
    setColor(RESET);
    return;
  }
  int* printTracker = (int*) malloc((graph -> numVertices) * sizeof(int));
  node* adjList;
  (graph -> queries[loc] -> searchNumber)++;
  for (i = 0; i < graph -> numVertices; i++)
    printTracker[i] = 0;
  printTracker[loc] = 1;
  setColor(GREEN);
  printf("People also looked for:\n");
  setColor(RESET);
  for (i = 0; i < graph -> numVertices && i < MAX_RECOM; i++) {
    adjList = graph -> adjLists[loc];
    maxQueries = 0;
    for (; adjList != NULL; adjList = adjList -> next) {
      j = adjList -> data -> id;
      if ((graph -> queries[j] -> searchNumber) >= maxQueries && !printTracker[j]) {
        toPrint = j;
        maxQueries = (graph -> queries[j] -> searchNumber);
      }
    }
    if (!printTracker[toPrint]) {
      printf("#%d %s\n", graph -> queries[toPrint] -> searchNumber, graph -> queries[toPrint] -> name);
      printTracker[toPrint] = 1;
    }
  }
  i = 0;
  while (!printTracker[i]) {
    i++;
  }
  if (i == (graph -> numVertices)) {
    printf("People didn't look for anything similar!\n");
  }
  setColor(RESET);
  printf("\n");
}

int searchEngine(Graph* graph) {
  char* input;
  greet();
  printf("> ");
  input = scanRaw(CHAR_LIMIT);
  if (!strcmp(input, "exit")) {
    free(input);
    return 0;
  }
  printLorem(input);
  printRecommendations(graph, input);
  free(input);
  return 1;
}

void printLorem(char* string) {
  printf("\n#1 '%s' is trending on social media\nLorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\n\n", string);
  printf("#2 Research hints that '%s' topic will be very important in the future.\nExcepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n\n",string);
}

int findInGraph(Graph* graph, char* string) {
  int i = 0;
  while ((i < (graph -> numVertices)) && strcmp(string, (graph -> queries[i] -> name)))
    i++;
  if (i == (graph -> numVertices)) {
    return -1;
  }
  return i;
}

void freeAdjList(node* adjList) {
  node* nodeHolder;
  if (!adjList) {
    return;
  }
  for (nodeHolder = adjList -> next; nodeHolder != NULL; adjList = nodeHolder, nodeHolder = (nodeHolder -> next)) {
    if (adjList) {
      free(adjList);
    }
  }
}

void mergeAdjLists(node** adjList1, node** adjList2, int* queryMap, Graph* graph) {
  node *current, *nodeHolder, *newNode;
  for (current = *adjList2; current != NULL; current = current -> next) {
    nodeHolder = *adjList1;
    while (nodeHolder != NULL && (nodeHolder -> vertex) != queryMap[current -> vertex])
      nodeHolder = nodeHolder -> next;
    if (!nodeHolder) {
      newNode = malloc(sizeof(node));
      newNode -> data = graph -> queries[queryMap[current -> vertex]];
      newNode -> vertex = queryMap[current -> vertex];
      newNode -> next = *adjList1;
      *adjList1 = newNode;
    }
  }
}

Graph* mergeGraphs(Graph* graph1, Graph* graph2) {
  int mutualQueries, i, j, *queryMap, index;
  node* nodeHolder;
  Graph* graph;
  mutualQueries = 0;
  queryMap = (int*) malloc((graph2 -> numVertices) * sizeof(int));
  for (i = 0; i < graph2 -> numVertices; i++)
    queryMap[i] = -1;
  for (i = 0, j = 0; i < graph1 -> numVertices; i++) {
    if ((index = findInGraph(graph2, graph1 -> queries[i] -> name)) != -1) {
      mutualQueries++;
      queryMap[index] = i;
    }
  }
  graph = createGraph(graph1 -> numVertices + graph2 -> numVertices - mutualQueries);
  for (i = 0; i < graph1 -> numVertices; i++) {
    graph -> queries[i] = createQuery(i, graph1 -> queries[i] -> name, 0);
    graph -> adjLists[i] = graph1 -> adjLists[i];
  }
  for (j = 0; j < graph2 -> numVertices; j++) {
    if (queryMap[j] == -1) {
      graph -> queries[i] = createQuery(i, graph2 -> queries[j] -> name, 0);
      queryMap[j] = i;
      i++;
    }
  }
  for (j=0,i=graph1 -> numVertices; j < graph2 -> numVertices; j++) {
    if (findInGraph(graph1, graph2 -> queries[j] -> name) != -1) {
      mergeAdjLists(&(graph -> adjLists[queryMap[j]]), &(graph2 -> adjLists[j]), queryMap, graph);
      freeAdjList(graph2 -> adjLists[j]);
    } else {
      graph -> adjLists[i] = graph2 -> adjLists[j];
      for (nodeHolder = graph -> adjLists[i]; nodeHolder != NULL; nodeHolder = nodeHolder -> next) {
        nodeHolder -> vertex = queryMap[nodeHolder -> vertex];
        nodeHolder -> data -> id = queryMap[nodeHolder -> data -> id];
      }
      i++;
    }
  }
  free(queryMap);
  return graph;
}

Graph* initializeGraph() {
  int size, i;
  Graph* graph;
  char* input;
  printf("How many queries (phrases / nodes) will you enter? ");
  size = getNumber();
  graph = createGraph(size);
  for (i = 0; i < size; i++) {
    printf("Enter query #%d: ", i);
    input = scanRaw(CHAR_LIMIT);
    graph -> queries[i] = createQuery(i, input, 0);
    free(input);
  }
  printf("To remind you here are the queries you entered:\n");
  printQueryList(graph -> queries, size);
  printf("Indicate similar queries by entering comma seperated query ids. (e.g. 1-2,3-5,3-2)\n");
  setColor(RED);
  printf("DO NOT repeat relations (e.g. 1-2,2-1). DO NOT leave whitespace (e.g. 1\t-2 , 1 -  3).\n");
  setColor(RESET);
  printf("Enter the relations (1-2,1-3,2-3): ");
  input = scanRaw(CHAR_LIMIT);
  handleRelationsString(graph, size, input);
  free(input);
  return graph;
}

void handleRelationsString(Graph* graph, int size, char* relationsString) {
  char* numberString = (char*) malloc(10 * sizeof(char));
  char* relation;
  int src, dest;
  relation = strtok(relationsString, ",");
  if (!strstr(relation, "-")) {
    free(numberString);
    return;
  }
  parseRelation(relation, numberString, &src, &dest);
  addEdge(graph, src, dest);
  int i=0;
  while ((relation = strtok(NULL, ",")) && i < 10) {
    parseRelation(relation, numberString, &src, &dest);
    addEdge(graph, src, dest);
  }
  free(numberString);
}

void parseRelation(char* relation, char* numberString, int* src, int* dest) {
  int i,j;
  i=0,j=0;
  while (relation[i] != '-') {
    if (relation[i] <= '9' && relation[i] >= '0') {
      numberString[j] = relation[i];
      j++;
    }
    i++;
  }
  numberString[j] = 0;
  *src = atoi(numberString);
  j=0;
  while (relation[i] != 0) {
    if (relation[i] <= '9' && relation[i] >= '0') {
      numberString[j] = relation[i];
      j++;
    }
    i++;
  }
  numberString[j] = 0;
  *dest = atoi(numberString);
}

query* findQueryAdd(Graph* graph, int v) {
  query** queriesList = graph -> queries;
  int numQueries = graph -> numVertices;
  int i = 0;
  while (i < numQueries && (queriesList[i] -> id != v))
    i++;
  if (i == numQueries) {
    return NULL;
  }
  return queriesList[i];
}

void printQueryList(query** queryList, int n) {
  int i;
  for (i = 0; i < n; i++) {
    printf("[#%d]: '%s'\n", queryList[i] -> id, queryList[i] -> name);
  }
}

query* createQuery(int idNum, char* queryString, int number) {
  query* newQuery = malloc(sizeof(struct query));
  newQuery -> id = idNum;
  strcpy(newQuery -> name, queryString);
  newQuery -> searchNumber = number;
  return newQuery;
}

node* createNode(Graph* graph, int v) {
  node* newNode = malloc(sizeof(node));
  newNode -> data = findQueryAdd(graph, v);
  newNode -> vertex = v;
  newNode -> next = NULL;
  return newNode;
};

void addEdge(Graph* graph, int src, int dest) {
  node* newNode = createNode(graph, dest);
  newNode -> next = graph-> adjLists[src];
  graph->adjLists[src] = newNode;
  newNode = createNode(graph, src);
  newNode->next = graph->adjLists[dest];
  graph->adjLists[dest] = newNode;
}

Graph* createGraph(int vertices) {
  int i;
  Graph* graph = malloc(sizeof(Graph));
  graph -> numVertices = vertices;
  graph -> adjLists = malloc(vertices * sizeof(node*));
  graph -> queries = malloc(vertices * sizeof(query*));
  for (i = 0; i < vertices; i++) {
    graph -> adjLists[i] = NULL;
    graph -> queries[i] = NULL;
  }
  return graph;
}

char* scanRaw(int size) {
  char* input = (char*) malloc((size + 1) * sizeof(char));
  fgets(input, size, stdin);
  strtok(input, "\n");
  return input;
}

int getNumber() {
  char* input = scanRaw(10);
  int num = atoi(input);
  free(input);
  return num;
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
