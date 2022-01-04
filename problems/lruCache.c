#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MY_CHAR_LIMIT 100
#define INIT_ARR_SIZE 2
#define INVALID UINT_MAX
#define DELETED UINT_MAX - 1
#define SEPERATOR "=============\n"

typedef struct {
    char id[MY_CHAR_LIMIT];
    char fname[MY_CHAR_LIMIT];
    char lname[MY_CHAR_LIMIT];
    int date;
    char address[MY_CHAR_LIMIT];
} Person;

typedef struct _Block {
    Person record;
    struct _Block* next;
    struct _Block* prev;
} Block;

typedef struct {
    size_t count;
    size_t size;
    Block *front, *rear;
} Cache;

typedef struct {
    char id[MY_CHAR_LIMIT];
    size_t loc;
} Entry;

typedef struct {
    size_t nmemb;
    Entry* map;
} Hashtable;

/* CACHE FUNCTIONS */
Cache* create_cache(size_t);
int add_to_cache(Cache*, Person, Hashtable*);
int move_to_front(Cache*, size_t, Hashtable*);
Person get_from_cache(Cache*, size_t loc);
void print_cache(Cache*);

/* QUEUE FUNCTIONS */
void dequeue(Cache*);

/* HASHMAP FUNCTIONS */
Hashtable* create_hashtable(size_t);
int insert_hashtable(Hashtable*, const char*, size_t);
size_t get_loc_hashtable(Hashtable*, const char*);
void print_hashtable(Hashtable*);

/* HASH FUNCTIONS */
long int horner_method(const char*);
size_t double_hash(long int, size_t, size_t, size_t);

/* PRIME NUMBER FUNCTIONS */
bool is_prime(size_t);
size_t smallest_prime_ge(size_t);

/* IO FUNCTIONS */
size_t get_cache_size();
Person* read_people_file(const char*, size_t*);

/* MEMORY FUNCTIONS */
void free_cache(Cache*);

int main(int argc, char const* argv[]) {
    size_t cachesize;
    size_t m;
    size_t size;
    size_t index;
    Person* people;
    /*
    for each record in file:
        index = hash(record.id)
        if map[index] is valid:
            log = retrieve_log(map[index])
            print_log(log)
        else:
            inert_to_cache(record)
            log = retrieve_log(map[index])
            print_log(log)
    */

    people = read_people_file(argv[1], &size);
    cachesize = get_cache_size();
    m = (size_t)(cachesize / 0.6 + 0.5);
    m = smallest_prime_ge(m);

    printf(SEPERATOR);
    printf("TABLE SIZE: %-3d\n", m);
    printf(SEPERATOR);
    Hashtable* table = create_hashtable(m);
    Cache* cache = create_cache(cachesize);

    for (size_t i = 0; i < size; i++) {
        printf("\n## REFERENCING '%-10s'\n", people[i].id);
        index = get_loc_hashtable(table, people[i].id);
        /* if not in hash table */
        if (index == INVALID || index == DELETED) {
            printf("Cache Miss. Caching '%s'...\n", people[i].id);
            insert_hashtable(table, people[i].id, 0);
            add_to_cache(cache, people[i], table);
            print_cache(cache);
            // print_hashtable(table);
        } else {
            printf("\nCache Hit. Updating cache...\n");
            move_to_front(cache, table->map[index].loc, table);
            print_cache(cache);
            // print_hashtable(table);
        }
    }

    free(table->map);
    free(table);
    free(people);
    free_cache(cache);
    return 0;
}

/* CACHE FUNCTIONS */
Cache* create_cache(size_t size) {
    Cache* c = (Cache*)malloc(sizeof(Cache));
    c->count = 0;
    c->size = size;
    c->front = NULL;
    c->rear = NULL;
    return c;
}

int add_to_cache(Cache* cache, Person p, Hashtable* table) {
    Block* temp;
    size_t i;

    /* remove first accessed block if cache is full */
    if (cache->count == cache->size) {
        i = get_loc_hashtable(table, cache->rear->record.id);
        table->map[i].loc = DELETED;
        dequeue(cache);
    }

    temp = (Block*)malloc(sizeof(Block));
    temp->record = p;
    temp->next = NULL;
    temp->prev = NULL;

    if (!(cache->rear)) {
        cache->rear = cache->front = temp;
    } else {
        cache->front->prev = temp;
        temp->next = cache->front;
        cache->front = temp;
    }

    i = get_loc_hashtable(table, p.id);
    table->map[i].loc = 0;
    cache->count++;

    for (temp = cache->front->next; temp; temp = temp->next) {
        i = get_loc_hashtable(table, temp->record.id);
        table->map[i].loc++;
    }

    return 0;
}

int move_to_front(Cache* cache, size_t loc, Hashtable* table) {
    Block* temp;
    size_t i, j;
    size_t index;

    if (loc == 0)
        return 0;

    i = 0;
    temp = cache->front;
    /* go to location in cache and update locations of
     * blocks that will be shifted */
    while (i < loc && temp->next) {
        index = get_loc_hashtable(table, temp->record.id);
        table->map[index].loc++;
        temp = temp->next, i++;
    }

    if (i != loc) {
        /* rollback location changes */
        for (j = 0, temp = cache->front; j < i; j++, temp = temp->next) {
            index = get_loc_hashtable(table, temp->record.id);
            table->map[index].loc--;
        }
        return -1;
    }

    /* remove node from queue */
    temp->prev->next = temp->next;

    if (temp->next) {
        temp->next->prev = temp->prev;
    } else {
        /* last element in queue */
        cache->rear = temp->prev;
    }

    /* assign temp's new links */
    temp->next = cache->front;
    temp->prev = NULL;

    /* put temp at the front of the queue */
    cache->front->prev = temp;
    cache->front = temp;

    /* change location of new block to 0 */
    index = get_loc_hashtable(table, temp->record.id);
    table->map[index].loc = 0;

    return 0;
}

void print_cache(Cache* c) {
    Block* temp;
    printf("\n= CACHE =\n");
    printf(SEPERATOR);
    for (temp = c->front; temp; temp = temp->next)
        printf("%-10s | %-10s | %-10s | %-4d | %-10s\n", temp->record.id, temp->record.fname, temp->record.lname, temp->record.date, temp->record.address);
    printf(SEPERATOR);
}

/* QUEUE FUNCTIONS */
void dequeue(Cache* cache) {
    Block* temp;
    /* do nothing if queue empty */
    if (!(cache->rear))
        return;

    if (cache->front == cache->rear)
        cache->front = NULL;

    temp = cache->rear;
    cache->rear = cache->rear->prev;

    if (cache->rear)
        cache->rear->next = NULL;

    free(temp);

    cache->count--;
}

/* HASHMAP FUNCTIONS */
Hashtable* create_hashtable(size_t size) {
    size_t i;
    Hashtable* table = (Hashtable*)malloc(sizeof(Hashtable));
    if (!table)
        return NULL;

    table->nmemb = size;
    table->map = (Entry*)malloc(size * sizeof(Entry));
    if (!(table->map))
        return NULL;

    /* set all entries in the hash table empty */
    for (i = 0; i < table->nmemb; i++)
        table->map[i].loc = INVALID;

    return table;
}

int insert_hashtable(Hashtable* table, const char* id, size_t loc) {
    size_t index, i;
    long int key;
    i = 0;
    key = horner_method(id);
    index = double_hash(key, i, table->nmemb, table->nmemb - 1);
    while (i < table->nmemb && table->map[index].loc != INVALID && table->map[index].loc != DELETED)
        index = double_hash(key, ++i, table->nmemb, table->nmemb - 1);

    if (i == table->nmemb)
        return -1;

    table->map[index].loc = loc;
    strcpy(table->map[index].id, id);
    return 0;
}

size_t get_loc_hashtable(Hashtable* table, const char* id) {
    size_t index, i;
    long int key;

    i = 0;
    key = horner_method(id);
    index = double_hash(key, i, table->nmemb, table->nmemb - 1);

    /* while table[index] is deleted and key != table[index].key */
    while (i < table->nmemb && (table->map[index].loc == DELETED || (table->map[index].loc < DELETED && strcmp(table->map[index].id, id))))
        index = double_hash(key, ++i, table->nmemb, table->nmemb - 1);

    if (table->map[index].loc == INVALID || strcmp(table->map[index].id, id))
        return INVALID;

    return index;
}

void print_hashtable(Hashtable* table) {
    size_t i;
    printf("\n= HASH TABLE =\n");
    printf("%-3s | %-10s | %-20s\n", "i", "loc", "id");
    printf(SEPERATOR);
    for (i = 0; i < table->nmemb; i++) {
        if (table->map[i].loc == INVALID) {
            printf("%-3ld | %-10ld | %-20s\n", i, -1, "INVALID");
        } else if (table->map[i].loc == DELETED) {
            printf("%-3ld | %-10ld | %-20s\n", i, -1, "DELETED");
        } else {
            printf("%-3ld | %-10ld | %-20s\n", i, table->map[i].loc, table->map[i].id);
        }
    }
}

/* HASH FUNCTIONS */
long int horner_method(const char* s) {
    size_t l;
    long int r, key;

    /* intial values for horner's method */
    l = strlen(s);
    r = 1;
    key = 0;

    /* key  = R^(l-1)*s[0]+R^(l-2)*s[1]+..+R^0*s[l-1] */
    while (l-- > 0) {
        key += s[l] * r;
        r *= 13;
    }

    return key;
}

/*
* Function: double_hash
* --------------------------
* calculates a hash of `key` less than `m`
*
* key: the number to be double hashed
* i: the number of tries for current key
* m: hash table size
* k: a number less than m
*
* returns: a hash unsigned integer
*/
size_t double_hash(long int key, size_t i, size_t m, size_t k) {
    long int h1, h2;
    h1 = key % m;
    h2 = 1 + (key % k);
    return (h1 + i * h2) % m;
}

/* PRIME NUMBER FUNCTIONS */
bool is_prime(size_t n) {
    size_t i = 5;
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;

    if (!(n % 2) || !(n % 3))
        return false;

    while (i * i <= n && (n % i || n % (i + 2)))
        i += 6;

    if (i * i > n)
        return true;

    return false;
}

/*
* Function: smallest_prime_ge
* --------------------------
* finds the smallest prime number greater
* or equal to `n`
*
* n: the number to start from
*
* returns: a prime number >= n
*/
size_t smallest_prime_ge(size_t n) {
    if (n < 3)
        return 2;

    if (n % 2 == 0)
        n++;

    while (!is_prime(n))
        n += 2;

    return n;
}

/* I/O Functions */

/*
* Function: get_cache_size
* --------------------------
* prompts the user to enter a non negative
* integer from user
*
* returns: the number from user input
*/
size_t get_cache_size() {
    char buffer[MY_CHAR_LIMIT];
    char* endptr;
    long int number;

    printf("Cache size: ");
    fgets(buffer, MY_CHAR_LIMIT, stdin);

    number = strtol(buffer, &endptr, 10);
    while (buffer == endptr || number < 0) {
        printf("[ERROR] Enter a non-negative integer value\n");
        printf("Cache size: ");
        fgets(buffer, MY_CHAR_LIMIT, stdin);

        number = strtol(buffer, &endptr, 10);
    }

    return (size_t)number;
}

Person* read_people_file(const char* filename, size_t* size) {
    FILE* fp;
    Person* people;
    size_t i = 0;
    size_t j = INIT_ARR_SIZE; /* ads array size */

    if (!(fp = fopen(filename, "rb"))) {
        printf("Couldn't read %s\nProbably doesn't exist\n", filename);
        return NULL;
    }

    printf("\n> Processing file %s\n", filename);
    people = (Person*)malloc(j * sizeof(Person));
    while (fscanf(fp, " %s %s %s %d %s", people[i].id, people[i].fname, people[i].lname, &(people[i].date), people[i].address) == 5) {
        i++;
        if (i >= j - 1) {
            j <<= 1;
            people = realloc(people, j * sizeof(Person));
        }
    }
    fclose(fp);
    *size = i;
    return people;
}

/* MEMORY FUNCTIONS */
void free_cache(Cache* c) {
    Block *b, *temp;
    if (!c)
        return;

    b = c->front;
    while ((temp = b->next)) {
        free(b);
        b = temp;
    }

    free(b);
    free(c);
}