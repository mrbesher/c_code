#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LINE_SIZE 100

int equalizeImage(char const**, bool);
void countPixels(int*, double*, int);
void makeCumulativeFreq(double*, double*, int);
void createPgm(int, int, char*);
void appendPixels(int*, FILE*, int, double*);
void skipComments(FILE*);
bool isAsciiPgm(FILE*);
bool isSpace(char);

int main(int argc, char const *argv[]) {
  int returnedValue;
  if (argc > 2) {
    returnedValue = equalizeImage(argv, 1);
  } else if (argc>1) {
    returnedValue = equalizeImage(argv, 0);
  } else {
    printf("Error: no file name provided.\n./equalize input.pgm [output.pgm]\n");
    return -1;
  }
  if (returnedValue != 0) {
    return -1;
  }
  return 0;
}

int equalizeImage(char const *argv[], bool providedOutputName) {
  int width, height, *pixelValues, i, pixelsNumber;
  double *cumulativeFreq;
  static double pixelCounter[256];
  FILE *picture, *pgmPicture;
  char *commentHolder, *fileName;
  fileName = (char*) malloc(64 * sizeof(char));
  commentHolder = (char*) malloc(100 * sizeof(char));
  pixelValues = (int*) malloc(500 * sizeof(int));
  if((picture = fopen(argv[1], "r")) == NULL) {
    printf("Error: Couldn't read %s\nProbably doesn't exist!\n", argv[1]);
    free(pixelValues);
    free(commentHolder);
    return -1;
  };
  if (!isAsciiPgm(picture)) {
    printf("%s is not an ASCII PGM file\n", fileName);
    fclose(picture);
    return -1;
  }
  if (providedOutputName) {
    strcpy(fileName, argv[2]);
  } else {
    strcpy(fileName, "equalized_");
    strcat(fileName, argv[1]);
  }
  skipComments(picture);
  fscanf(picture, "%d %d", &width, &height);
  skipComments(picture);
  fscanf(picture, "%d", &pixelsNumber);
  fgetc(picture);
  pixelsNumber = width*height;
  printf("Calculating frequency...\n");
  do {
    i=0;
    do {
      fscanf(picture, "%d", pixelValues + i);
      printf("GOT: %d", pixelValues[i]);
      i++;
    } while((fgetc(picture) != EOF) && i<500);
    countPixels(pixelValues, pixelCounter, i);
  } while(i==500);
  cumulativeFreq = (double*) malloc(256 * sizeof(double));
  printf("Calculating cumulative frequency...\n");
  makeCumulativeFreq(pixelCounter, cumulativeFreq, pixelsNumber);
  createPgm(width, height, fileName);
  //get the pixels back from start and pass them to write a file
  printf("Creating %s as an output...\n", fileName);
  pgmPicture = fopen(fileName, "a");
  rewind(picture);
  //remove comments and type specifiers
  fscanf(picture, "%s", commentHolder);
  fgetc(picture);
  do {
    fscanf(picture, "%[^\n]", commentHolder);
    fgetc(picture);
  } while(commentHolder[0] == '#');
  fscanf(picture, " %s", commentHolder);
  do {
    i=0;
    do {
      fscanf(picture, "%d", pixelValues + i);
      i++;
    } while((fgetc(picture) != EOF) && i<500);
    appendPixels(pixelValues, pgmPicture, i, cumulativeFreq);
  } while(i==500);
  fclose(pgmPicture);
  free(fileName);
  free(cumulativeFreq);
  fclose(picture);
  free(pixelValues);
  free(commentHolder);
  return 0;
}

void countPixels(int* pixelValues, double pixelCounter[], int size) {
  for (size_t i = 0; i < size; i++)
    pixelCounter[pixelValues[i]]++;
}

void makeCumulativeFreq(double* pixelCounter, double* cumulativeFreq, int pixelsNumber) {
  //normalize the number of every pixel
  for (size_t i = 0; i < 256; i++) {
    pixelCounter[i] /= pixelsNumber;
  }
  cumulativeFreq[0] = pixelCounter[0];
  for (size_t i = 0; i < 255; i++) {
    cumulativeFreq[i+1] = cumulativeFreq[i] + pixelCounter[i+1];
  }
}

void createPgm(int width, int height, char fileName[]) {
  FILE* pgmPicture;
  pgmPicture = fopen(fileName, "w");
  fprintf(pgmPicture, "P2\n%d %d\n255\n", width, height);
  fclose(pgmPicture);
}

void appendPixels(int* pixelValues, FILE* pgmPicture, int size, double* cumulativeFreq) {
  for (size_t i = 0; i < size; i++) {
    fprintf(pgmPicture, "%d\n", (int)(cumulativeFreq[pixelValues[i]]*255+0.5));
  }
}

void skipComments(FILE* file) {
  int c;
  char* lineHolder = (char*) malloc(LINE_SIZE * sizeof(char));
  while ((c = fgetc(file)) && isSpace(c));
  if (c == '#') {
    fgets(lineHolder, LINE_SIZE, file);
    skipComments(file);
  } else {
    fseek(file, -1, SEEK_CUR);
  }
  free(lineHolder);
}

bool isAsciiPgm(FILE* file) {
  char* typeHolder = (char*) malloc(LINE_SIZE * sizeof(char));
  fscanf(file, " %s ", typeHolder);
  if (strcmp(typeHolder, "P2")) {
    free(typeHolder);
    return false;
  }
  free(typeHolder);
  return true;
}

bool isSpace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}
