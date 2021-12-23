#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CITY_CHAR_LIMIT 25
#define INIT_ARR_SIZE 2

typedef struct {
    char src[CITY_CHAR_LIMIT];
    char dest[CITY_CHAR_LIMIT];
    int hours;
    int mins;
    int price;
} Flight;

typedef struct {
    int price;
    int duration;
} Cost;

typedef struct {
    char **mappings;
    Cost **adjMatrix;
    size_t nmemb; /* number of cities in the graph */
} Graph;

typedef struct {
    char src[CITY_CHAR_LIMIT];
    char dest[CITY_CHAR_LIMIT];
    int criterion; /* 0: price, 1: time */
    size_t transitlimit;
} Preference;

typedef struct _Citynode {
    int cityId;
    struct _Citynode *next;
} Citynode;

typedef struct _Path {
    Cost *cost;
    Citynode *head;
    struct _Path *next;
} Path;

/* GRAPH FUNCTIONS */

Graph *create_flights_graph(Flight *, size_t);
char **create_mappings(Flight *, size_t, size_t *);
size_t get_city_id(char *, char **, size_t);

/* PATH FUNCTIONS */

Path get_all_paths_sorted(Graph *, Preference);
bool is_city_in_path(int, Citynode *);
int add_full_path(Path *, Citynode *, Graph *);
int add_all_paths(Graph *, Path *, int, int);
int add_paths_from_node(Graph *, Path *, Citynode *, int);

/* STACK FUNCTONS */
int remove_head(Citynode *);
int add_city_to_stack(Citynode *, int);

/* IO FUNCTIONS */

Flight *read_flights_file(const char *, size_t *);
Preference get_pref_usr();

/* MEMORY FUNCTIONS */

void free_graph(Graph *);

/* GENERIC FUNCTIONS */

bool is_str_in_arr(char[], char **, size_t);

int main(int argc, char const *argv[]) {
    Flight *flights;    /* An array containing flights read from user */
    Graph *graph;       /* A graph containing all connections and ids of cities */
    Preference usrPref; /* user choices for the flight */
    Path *paths = NULL;
    size_t inputsize, i, j;

    if (argc > 1) {
        flights = read_flights_file(argv[1], &inputsize);
    } else {
        printf("Error: No argument provided. Run like this\n./flightRouteFinder flights.txt\n");
        return 1;
    }

    if (!flights) {
        printf("%d > Error: Couldn't create / read flights.", __LINE__);
        return 1;
    }

    printf("src des hr mi price\n");
    for (i = 0; i < inputsize; i++) {
        printf("%s %s %2d %2d %4d\n", flights[i].src, flights[i].dest,
               flights[i].hours, flights[i].mins, flights[i].price);
    }

    graph = create_flights_graph(flights, inputsize);
    if (!graph) {
        printf("%d > Error: Couldn't create graph.", __LINE__);
        free(flights);
        return 1;
    }

    for (i = 0; i < graph->nmemb; i++) {
        for (j = 0; j < graph->nmemb; j++) {
            printf("%4d\t", graph->adjMatrix[i][j].duration);
        }
        printf("\n");
    }

    usrPref = get_pref_usr();
    /*
    paths = get_all_paths_sorted(graph, usrPref);
    */

    add_all_paths(graph, paths, get_city_id(usrPref.src, graph->mappings, graph->nmemb),
                  get_city_id(usrPref.dest, graph->mappings, graph->nmemb));
    free(flights);
    free_graph(graph);
    return 0;
}

/* GRAPH TRAVERSE FUNCTIONS */
int add_all_paths(Graph *graph, Path *paths, int srcId, int destId) {
    paths = (Path*) malloc(sizeof(Path));
    Citynode *visited = NULL;
    paths->head = NULL;
    add_city_to_stack(visited, srcId);
    return add_paths_from_node(graph, paths, visited, destId);
}

int add_paths_from_node(Graph *graph, Path *paths, Citynode *visited, int destId) {
    Citynode *currCity = paths->head;
    size_t i;
    Cost connCost;

    for (i = 0; i < graph->nmemb; i++) {
        connCost = graph->adjMatrix[currCity->cityId][i];

        /* if a flight exists between current city and dest */
        if (i == (size_t)destId && connCost.price != 0) {
            printf("Found dest from %ld to %d\n", i, destId);
            add_city_to_stack(visited, i);
            add_full_path(paths, visited, graph);
            remove_head(visited);
        } else if (connCost.price != 0 && !is_city_in_path(currCity->cityId, visited)) {
            /* if city has a connection and not visited */
            printf("Visiting %ld", i);
            add_city_to_stack(visited, i);
            add_paths_from_node(graph, paths, visited, destId);
        }
    }
    /* pop self from stack */
    printf("backtracking from %d", visited->cityId);
    remove_head(visited);
    return 0;

    /*
    currCity = get_head(visited);
    for neighbor in currCity.neighbors:
        if neighbor == destId:
            break;
        if not visited:
            add_to_visited(visited, currCity);
            add_paths_from_node(graph, paths, visited, destId);
    if neighbor == destId:
        add_path(Paths, visited)
    reutrn 0;
    */
}

int add_full_path(Path *paths, Citynode *visited, Graph *graph) {
    Path *temppaths = paths;
    Citynode *tempcity = visited;
    Citynode *newcity;
    Path *currPath;
    Cost *cost, tempcost;

    currPath = malloc(sizeof(Path));
    cost = malloc(sizeof(Cost));
    cost->duration = 0;
    cost->price = 0;

    newcity = (Citynode *)malloc(sizeof(Citynode));
    newcity->cityId = tempcity->cityId;
    newcity->next = NULL;
    currPath->head = newcity;

    /* add cities to currPath */
    while ((tempcity = tempcity->next)) {
        /* add current link cost */
        tempcost = graph->adjMatrix[newcity->cityId][tempcity->next->cityId];
        currPath->cost->price += tempcost.price;
        currPath->cost->duration += tempcost.duration + 1;

        /* add new node to path */
        newcity = (Citynode *)malloc(sizeof(Citynode));
        newcity->cityId = tempcity->cityId;
        newcity->next = currPath->head;
    }

    /*
    number of transits is `nodes - 2`
    one city is not counted above already
    */
    currPath->cost->duration--;

    /* linkedlist empty */
    if (!temppaths) {
        paths = currPath;
        currPath->next = NULL;
        return 0;
    }

    /* path has only one node */
    if (!(temppaths->next)) {
        /* if cost of current path is more */
        if (temppaths->cost->price > currPath->cost->price) {
            currPath->next = temppaths;
        } else {
            temppaths->next = currPath;
            currPath->next = NULL;
        }
        return 0;
    }

    /* calculated path has less cost than the head path */
    if (temppaths->cost->price > currPath->cost->price) {
        currPath->next = temppaths;
        return 0;
    }

    while (temppaths->next && temppaths->next->cost->price < currPath->cost->price) {
        temppaths = temppaths->next;
    }

    currPath->next = temppaths->next;
    temppaths->next = currPath;
    return 0;
}

int add_city_to_stack(Citynode *visited, int id) {
    Citynode *city = malloc(sizeof(Citynode));
    if (!city)
        return -1;

    /* intialize Citynode */
    city->cityId = id;
    city->next = visited;
    visited = city;

    return 0;
}

int remove_head(Citynode *path) {
    Citynode *temp = path;
    if (!temp)
        return -1;

    path = path->next;
    free(temp);
    return 0;
}

/* GRAPH STRUCTURAL FUNCTIONS */
Graph *create_flights_graph(Flight *flights, size_t inputsize) {
    size_t i;
    size_t row, col;
    Graph *graph;

    graph = (Graph *)malloc(sizeof(Graph));

    graph->mappings = create_mappings(flights, inputsize, &(graph->nmemb));

    graph->adjMatrix = (Cost **)malloc(graph->nmemb * sizeof(Cost *));
    /* set all costs to 0 indicating no flight and allocate memory */
    for (i = 0; i < graph->nmemb; i++)
        graph->adjMatrix[i] = (Cost *)calloc((graph->nmemb), sizeof(Cost));

    for (i = 0; i < inputsize; i++) {
        row = get_city_id(flights[i].src, graph->mappings, graph->nmemb);
        col = get_city_id(flights[i].dest, graph->mappings, graph->nmemb);
        if (row == UINT_MAX || col == UINT_MAX) {
            free_graph(graph);
            printf("%d > [FATAL ERROR] City name not found. Although just inserted!\n", __LINE__);
            return NULL;
        }
        graph->adjMatrix[row][col].duration = flights[i].hours * 60 + flights[i].mins;
        graph->adjMatrix[row][col].price = flights[i].price;
        graph->adjMatrix[col][row] = graph->adjMatrix[row][col];
    }
    return graph;
}

/*
* returns the id of the city string
* returns UINT_MAX on failure
*/
size_t get_city_id(char *city, char **cities, size_t nmemb) {
    size_t i;
    for (i = 0; i < nmemb; i++) {
        if (!strcmp(city, cities[i])) {
            return i;
        }
    }
    return UINT_MAX;
}

/* returns true if `cityId` is in path */
bool is_city_in_path(int keyCity, Citynode *path) {
    Citynode *temp = path;
    while (temp && temp->cityId != keyCity)
        temp = temp->next;

    if (temp->cityId == keyCity)
        return true;

    return false;
}

char **create_mappings(Flight *flights, size_t inputsize, size_t *nmemb) {
    size_t i, arrSize;
    char **mappings;

    arrSize = INIT_ARR_SIZE;
    mappings = (char **)malloc(arrSize * sizeof(char *));
    *nmemb = 0;

    for (i = 0; i < inputsize; i++) {
        /* if flights[i].src not in mappings add it */
        if (!is_str_in_arr(flights[i].src, mappings, *nmemb)) {
            mappings[*nmemb] = (char *)malloc(CITY_CHAR_LIMIT * sizeof(char));
            strcpy(mappings[(*nmemb)++], flights[i].src);
        }

        /* if flights[i].dest not in mappings add it */
        if (!is_str_in_arr(flights[i].dest, mappings, *nmemb)) {
            mappings[*nmemb] = (char *)malloc(CITY_CHAR_LIMIT * sizeof(char));
            strcpy(mappings[(*nmemb)++], flights[i].dest);
        }

        if (*nmemb + 2 > arrSize) {
            arrSize <<= 1;
            mappings = (char **)realloc(mappings, arrSize * sizeof(char *));
        }
    }

    return mappings;
}

/* GENERIC FUNCTIONS */

bool is_str_in_arr(char key[], char **strarr, size_t nmemb) {
    size_t i;
    for (i = 0; i < nmemb; i++) {
        if (!strcmp(key, strarr[i]))
            return true;
    }

    return false;
}

/* IO FUNCTIONS */

Flight *read_flights_file(const char *filename, size_t *size) {
    FILE *fp;
    Flight *flights;
    size_t i = 0;
    size_t j = INIT_ARR_SIZE; /* ads array size */

    if (!(fp = fopen(filename, "rb"))) {
        printf("Couldn't read %s\nProbably doesn't exist\n", filename);
        return NULL;
    }

    printf("\n> Processing file %s\n", filename);
    flights = (Flight *)malloc(j * sizeof(Flight));
    while (fscanf(fp, " %s %s %d %d %d", flights[i].src, flights[i].dest, &flights[i].hours,
                  &flights[i].mins, &flights[i].price) == 5) {
        i++;
        if (i >= j - 1) {
            j <<= 1;
            flights = realloc(flights, j * sizeof(Flight));
        }
    }
    fclose(fp);
    *size = i;
    return flights;
}

Preference get_pref_usr() {
    Preference pref;
    char ans[CITY_CHAR_LIMIT];
    /* get departure city and remove trailing white space */
    printf("Departure city: ");
    fgets(pref.src, CITY_CHAR_LIMIT, stdin);
    strtok(pref.src, "\n");
    strtok(pref.src, "\r");

    /* get destination city and remove trailing white space */
    printf("Destination: ");
    fgets(pref.dest, CITY_CHAR_LIMIT, stdin);
    strtok(pref.dest, "\n");
    strtok(pref.dest, "\r");

    printf("Max # of transits: ");
    scanf(" %lu", &pref.transitlimit);

    printf("(p)rice / (t)ime: ");
    scanf(" %s", ans);
    pref.criterion = (ans[0] == 't' || ans[0] == 'T') ? 1 : 0;

    return pref;
}

/* memory functions */

void free_graph(Graph *a) {
    size_t i;

    for (i = 0; i < a->nmemb; i++)
        free(a->mappings[i]);

    free(a->mappings);
    /* free all matrices rows */
    for (i = 0; i < a->nmemb; i++)
        free(a->adjMatrix[i]);
    free(a->adjMatrix);
    free(a);
}