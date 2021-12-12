#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEPERATOR "===========================\n"
#define INIT_ARR_SIZE 2

typedef struct {
    int id;
    int startTime;
    int duration;
    int value;
} Advert;

typedef struct {
    int value;
    int lNonConflicting;
} Profit;

/* optimal solution function */

Profit *create_max_profit_arr(Advert *, size_t);
int get_last_nonconflict(Advert *, size_t, size_t, size_t);

/* IO functions */

Advert *get_ads_from_usr(size_t);
Advert *read_ads_file(const char *, size_t *);
void print_ads(Advert *, size_t);
void print_optimal_solution(Profit *, Advert *, size_t);

/* sort functions */

int cmp_ads_finish(const void *, const void *);
void swap(void *, void *, size_t);
size_t partition(void *, size_t, size_t, int (*)(const void *, const void *));
void myqsort(void *, size_t, size_t, int (*)(const void *, const void *));

/* generic functions */

int max(int, int);

int main(int argc, char const *argv[]) {
    Advert *ads;
    size_t size;
    Profit *profits;

    if (argc > 1) {
        ads = read_ads_file(argv[1], &size);
    } else {
        /* get adverts from user */
        printf("# of ads: ");
        scanf(" %lu", &size);
        ads = get_ads_from_usr(size);
    }

    if (!ads) {
        printf("%d > Error: Couldn't create ads array.", __LINE__);
        return 1;
    }

    /* sort data by finish time */
    myqsort(ads, size, sizeof(Advert), cmp_ads_finish);

    profits = create_max_profit_arr(ads, size);

    if (!profits) {
        printf("%d > Error: Couldn't create profits array.", __LINE__);
        return 1;
    }

    print_optimal_solution(profits, ads, size);

    free(ads);
    free(profits);
    return 0;
}

/* OPTIMAL SOLUTION FUNCTIONS */

/*
* Function: create_max_profit_arr
* --------------------------
* calculates a profits array that has the highest profit possible
* considering ads (1..i) in arr[i]
*
* ads: the array of ads to consider
* size: the number of ads in array
*
* returns: profits array with max profit and last non conflicting
* ad in each array slot
*/
Profit *create_max_profit_arr(Advert *ads, size_t size) {
    size_t i;
    Profit *profits;
    int nonconflict, curValue;

    profits = (Profit *)malloc(size * sizeof(Profit));

    /* malloc failed */
    if (!profits)
        return profits;

    profits[0].value = ads[0].value;
    profits[0].lNonConflicting = -1;

    for (i = 1; i < size; i++) {
        nonconflict = get_last_nonconflict(ads, i, 0, i);
        curValue = (nonconflict == -1) ? ads[i].value : ads[i].value + profits[nonconflict].value;

        /* if including the current ad is more profitable set profit[i] to the new profit
           else continue with the prev ads */
        if (max(profits[i - 1].value, curValue) == curValue) {
            profits[i].value = curValue;
            profits[i].lNonConflicting = nonconflict;
        } else {
            profits[i] = profits[i - 1];
        }
    }
    return profits;
}

/*
* Function: get_last_nonconflict
* --------------------------
* calculates the last non conflicting Advert index before
* the key Advert
*
* ads: the array of ads to consider
* index: the index of the key Advert
* l: the offset to start looking for a non-conflicting Advert (inclusive)
* r: the limit to stop looking for a non-conflicting Advert (exclusive)
* 
* returns: the index of the last non-conflicting Advert on success
*          returns -1 on failure
*/
int get_last_nonconflict(Advert *ads, size_t index, size_t l, size_t r) {
    size_t mid;

    if (l >= r)
        return -1;

    mid = (l + r) / 2;

    /* if ad[mid] conflicts search in ads before mid */
    if (ads[mid].startTime + ads[mid].duration > ads[index].startTime) {
        return get_last_nonconflict(ads, index, l, mid);
    }

    /* check if this is the last job not conclicting */
    if (ads[mid + 1].startTime + ads[mid + 1].duration <= ads[index].startTime) {
        return get_last_nonconflict(ads, index, mid + 1, r);
    } else {
        return mid;
    }
}

/* IO FUNCTIONS */

/*
* Function: get_ads_from_usr
* --------------------------
* gets Advert array from user
*
* size: the number of elements to get from user
*
* returns: a pointer to the array containing Advert array read from user
*/
Advert *get_ads_from_usr(size_t size) {
    size_t i;
    Advert *ads = (Advert *)malloc(size * sizeof(Advert));

    /* malloc failed */
    if (!ads)
        return ads;

    for (i = 0; i < size; i++) {
        printf("#%lu Enter starttime duration value: ", i + 1);
        scanf(" %d %d %d", &ads[i].startTime, &ads[i].duration, &ads[i].value);
        ads[i].id = i + 1;
    }
    return ads;
}

Advert *read_ads_file(const char *filename, size_t *size) {
    FILE *adsfp;
    Advert *ads;
    size_t i = 0;
    size_t j = INIT_ARR_SIZE; /* ads array size */
    if (!(adsfp = fopen(filename, "rb"))) {
        printf("Couldn't read %s\nProbably doesn't exist\n", filename);
        return NULL;
    }

    printf("\n> Processing file %s\n", filename);
    ads = (Advert *)malloc(j * sizeof(Advert));
    while (fscanf(adsfp, " %d %d %d", &ads[i].startTime, &ads[i].duration, &ads[i].value) == 3) {
        ads[i].id = i + 1;
        i++;
        if (i >= j - 1) {
            j <<= 1;
            ads = realloc(ads, j * sizeof(Advert));
        }
    }
    fclose(adsfp);
    *size = i;
    return ads;
}

void print_ads(Advert *ads, size_t size) {
    size_t i;
    printf(SEPERATOR);
    printf("#ID\t\tStart\t\tDur.\t\tVal.\t\tFin.\n");
    printf(SEPERATOR);
    for (i = 0; i < size; i++) {
        printf("#%3d\t\t%d\t\t%d\t\t%d\t\t%d\n", ads[i].id, ads[i].startTime, ads[i].duration, ads[i].value,
               ads[i].startTime + ads[i].duration);
    }
}

void print_optimal_solution(Profit *profits, Advert *ads, size_t size) {
    size_t i = 0; /* to iterate in optimal solution */
    int index = (int)size - 1;
    Advert *optimal = (Advert *)malloc(size * sizeof(Advert));

    if (!optimal) {
        printf("%d > Error: Cannot allocate memory for optimal solution array", __LINE__);
        return;
    }

    while (index > 0) {
        if (profits[index].value != profits[index - 1].value) {
            optimal[i++] = ads[index];
            index = profits[index].lNonConflicting;
        } else {
            index--;
        }
    }

    /* if ad #0 is the last non-conflicting */
    if (index == 0)
        optimal[i++] = ads[index];

    printf("\nThe following ads would give you the highest profit (%d):\n", profits[size - 1].value);
    print_ads(optimal, i);
    free(optimal);
}

/* SORT FUNCTIONS */

/*
* Function: cmp_ads_finish
* --------------------------
* compares two Adverts according to their end time
*
* a: a pointer to the first Advert
* b: a pointer to the other Advert
*
* returns: the difference between end time of a and b
*/
int cmp_ads_finish(const void *a, const void *b) {
    int finishA = ((Advert *)a)->startTime + ((Advert *)a)->duration;
    int finishB = ((Advert *)b)->startTime + ((Advert *)b)->duration;
    return finishA - finishB;
}

void swap(void *a, void *b, size_t size) {
    void *temp = malloc(size);
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
}

size_t partition(void *base, size_t nmemb, size_t size,
                 int (*compar)(const void *, const void *)) {
    void *pivot = (char *)base + (nmemb - 1) * size; /* last element as a pivot */
    int i = -1;                                      /* position the pivot should be in */
    size_t j;                                        /* loop variable */

    for (j = 0; j < nmemb - 1; j++) {
        /* if compar says pivot is greater swap */
        if (compar((char *)base + j * size, pivot) < 0)
            i++, swap((char *)base + i * size, (char *)base + j * size, size);
    }
    /* swap pivot with its calculated location */
    swap((char *)base + (++i) * size, pivot, size);
    return (size_t)i;
}

void myqsort(void *base, size_t nmemb, size_t size,
             int (*compar)(const void *, const void *)) {
    size_t offset;
    if (nmemb < 2)
        return;
    offset = partition(base, nmemb, size, compar);
    myqsort(base, offset, size, compar);
    myqsort((char *)base + (offset + 1) * size, nmemb - (offset + 1), size, compar);
}

/* GENERIC FUNCTIONS */
int max(int a, int b) {
    return (a > b) ? a : b;
}