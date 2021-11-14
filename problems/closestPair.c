#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <math.h>

// constants
#define INIT_ARR_SIZE 2

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
  double distance;
  size_t size;
  Point* data;
} PointsArr;

// sort functions
int comparPointsByX(const void*, const void*);
int comparPointsByY(const void*, const void*);

// generic functions
PointsArr* allocPointsArr(size_t);
void freePointsArr(PointsArr*);

// point functions
PointsArr* readPointsFromFile(const char*);
PointsArr* findClosestPair(PointsArr*);
PointsArr* bfClosestPair(PointsArr*);
PointsArr* closestNearMedian(PointsArr*);
double euclideanDist(Point, Point);
PointsArr* minPairByDist(PointsArr*, PointsArr*);



int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("ERROR: no filename. run like this:\nclosestpair input.txt\n");
    return 1;
  }
  PointsArr* arr=readPointsFromFile(argv[1]); // holds points stored in txt file
  PointsArr* closest; // will store the closestpair
  if (!arr)
    return 1;
  qsort(arr->data, arr->size, sizeof(Point), comparPointsByX);
  closest=findClosestPair(arr);
  printf("Closest pair: (%d,%d) and (%d,%d)\nDistance: %lf\n", closest->data[0].x, closest->data[0].y,
          closest->data[1].x, closest->data[1].y, closest->distance);
  freePointsArr(arr);
  freePointsArr(closest);
  return 0;
}


/*
* Function: readPointsFromFile
* --------------------------
* reads points from filename as (x,y) pairs
*
* filename: a char pointer to the name of the points file
*
* returns: a pointer to the newly allocated struct with points arr and size
*/
PointsArr* readPointsFromFile(const char* filename) {
  FILE *pointsfp; // input file fp
  if (!(pointsfp = fopen(filename, "rb"))) {
    printf("Couldn't read %s\nProbably doesn't exist\n", filename);
    return NULL;
  }
  printf("Processing file %s\n", filename);
  size_t i=0; // counter
  size_t j=INIT_ARR_SIZE; // points arr size
  PointsArr *points = allocPointsArr(j);
  while (fscanf(pointsfp, " %d %d", &((points->data[i]).x), &((points->data)[i].y))==2) {
    i++;
    if (i>=j-1) {
      j<<=1;
      points->data = realloc(points->data, j*sizeof(Point));
    }
  }
  fclose(pointsfp);
  points->size=i;
  return points;
}


/*
* Function: findClosestPair
* --------------------------
* finds the closest pair in the list using a divide and conquer technique
*
* arr: a struct containing the array to be processed
*
* returns: a pointer to a struct containing the closest pair
*/
PointsArr* findClosestPair(PointsArr* arr) {
  if (arr->size <= 3)
    return bfClosestPair(arr);
  size_t median = arr->size / 2; // floors if arr->size is not even

  // initialize an array for the left half and get the closestpair from it
  PointsArr lHalf;
  PointsArr* lclosestPair; // will be set to the closestpair in the left half
  lHalf.size = median;
  lHalf.data = arr->data;
  lclosestPair = findClosestPair(&lHalf); // pair distance minDl

  // initialize an array for the right half and get the closestpair from it
  PointsArr rHalf;
  PointsArr* rclosestPair; // will be set to the closestpair in the right half
  rHalf.size = arr->size - median;
  rHalf.data = arr->data+median;
  rclosestPair = findClosestPair(&rHalf); // pair distance minDr

  PointsArr* closestpair = minPairByDist(lclosestPair, rclosestPair);

  // check if an inter-region closer pair exists
  PointsArr* nearMedian = allocPointsArr(arr->size);
  size_t i, j=0;
  for (i = 0; i < arr->size; i++) {
    if ((arr->data[median].x - arr->data[i].x) < closestpair->distance){
      nearMedian->data[j++]=arr->data[i]; // if the point falls within 2d
    }
  }
  nearMedian->size=j;
  // if closest pair is the closest one it won't change else it is freed and
  // replaced with one from the 2d area
  if (nearMedian->size > 1)
    closestpair = minPairByDist(closestpair, closestNearMedian(nearMedian));
  freePointsArr(nearMedian);

  return closestpair;
}

/*
* Function: bfClosestPair
* --------------------------
* finds the closest pair in the list by checking all possible pairs
*
* arr: a struct containing the array to be processed
*
* returns: a pointer to a struct containing the closest pair
*/
PointsArr* bfClosestPair(PointsArr* arr) {
  size_t i, j;
  double currMinDist, minDistance = INT_MAX; // will be used to compare with 'infinity' when starting
  PointsArr* closestpair = allocPointsArr(2); // an array that has closest pair
  for (i = 0; i < arr->size; i++) {
    for (j = i+1; j < arr->size; j++) {
      if ((currMinDist = euclideanDist(arr->data[i], arr-> data[j])) < minDistance) {
        minDistance = currMinDist;
        closestpair->data[0] = (arr->data)[i];
        closestpair->data[1] = (arr->data)[j];
        closestpair->distance=currMinDist;
      }
    }
  }
  return closestpair;
}


/*
* Function: closestNearMedian
* --------------------------
* finds the closest pair in the list after sorting according to y-axis
*
* arr: a struct containing the array in a d distance away from median
*
* returns: a pointer to a struct containing the closest inter-region pair
*/
PointsArr* closestNearMedian(PointsArr* arr) {
  qsort(arr->data, arr->size, sizeof(Point), comparPointsByY);
  return bfClosestPair(arr);
}


/*
* Function: minPairByDist
* --------------------------
* finds the pair with a smaller distance from a and b
*
* a: a struct containing the first pair to be compared
* b: a struct containing the second pair to be compared
*
* returns: a pointer to a the pair with the min distance
*/
PointsArr* minPairByDist(PointsArr* a, PointsArr* b) {
  if (a->distance > b->distance) {
    freePointsArr(a); // freeing outside the function requires knowing which pair is chosen
    return b;
  }
  freePointsArr(b);
  return a;
}


double euclideanDist(Point a, Point b) {
  return sqrt((double)((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)));
}


int comparPointsByX(const void *a, const void *b) {
  return (((Point*)a)->x - ((Point*)b)->x);
}


int comparPointsByY(const void *a, const void *b) {
  return (((Point*)a)->y - ((Point*)b)->y);
}


PointsArr* allocPointsArr(size_t nmemb) {
  PointsArr* arr = (PointsArr*) malloc(sizeof(PointsArr)); // a struct that holds points array and its info
  arr->size = nmemb;
  arr->data = (Point*) malloc((nmemb)*sizeof(Point));
  return arr;
}


void freePointsArr(PointsArr* arr) {
  free(arr->data);
  free(arr);
}
