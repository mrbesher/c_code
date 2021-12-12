#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comparInt(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

void printArr(int *arr, size_t nmemb) {
    for (size_t i = 0; i < nmemb; i++) {
        printf("%d\t", arr[i]);
    }
    printf("\n");
}

void merge(void *dest, void *srca, size_t nmemba, void *srcb, size_t nmembb, size_t size,
           int (*compar)(const void *, const void *)) {
    size_t i = 0, j = 0, k = 0;

    /* do while none of the source arrays is consumed */
    while (i < nmemba && j < nmembb) {
        /* if compar says srca[i] is smaller copy it to dest[k] else copy srcb[j] */
        if (compar((char *)srca + (i * size), (char *)srcb + (j * size)) <= 0) {
            /* srca[i] <= src[j]: dest[k] = srca[i] */
            memcpy((char *)dest + (k * size), (char *)srca + (i * size), size);
            i++;
        } else {
            /* srca[i] > src[j]: dest[k] = srcb[j] */
            memcpy((char *)dest + (k * size), (char *)srcb + (j * size), size);
            j++;
        }
        k++;
    }

    if (i == nmemba) {
        /* srca is consumed copy srcb[j..nmembb] to dest[k..] */
        memcpy((char *)dest + (k * size), (char *)srcb + (j * size), (nmembb - j) * size);
    } else {
        /* srcb is consumed copy srca[i..nmemba] to dest[k..] */
        memcpy((char *)dest + (k * size), (char *)srca + (i * size), (nmemba - i) * size);
    }
}

void mergesort(void *base, size_t nmemb, size_t size,
               int (*compar)(const void *, const void *)) {
    size_t mid;
    void *a, *b;

    if (nmemb < 2)
        return;

    mid = nmemb / 2;

    /* divide the array into 2 halves */
    a = malloc(mid * size);
    b = malloc((nmemb - mid) * size);

    memcpy(a, base, mid * size);
    memcpy(b, (char *)base + (mid * size), (nmemb - mid) * size);

    /* sort each half */
    mergesort(a, mid, size, compar);
    mergesort(b, nmemb - mid, size, compar);

    merge(base, a, mid, b, nmemb - mid, size, compar);

    free(a);
    free(b);
}

int main() {
    int arr[10] = {10, 4, 1, 11, 3, 6, 1, 19, 0, 12};
    mergesort(arr, 10, sizeof(int), comparInt);
    printArr(arr, 10);
    return 0;
}