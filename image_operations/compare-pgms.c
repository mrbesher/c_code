#ifdef _WIN32
#include <windows.h> //import windows.h to change console color
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define RESET 0
// char lengths
#define INPUT_SIZE 500
#define LINE_SIZE 100

void setColor(int);

bool isSpace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
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

bool isBinaryPgm(FILE* file) {
  char* typeHolder = (char*) malloc(LINE_SIZE * sizeof(char));
  fscanf(file, " %s ", typeHolder);
  if (strcmp(typeHolder, "P5")) {
    free(typeHolder);
    return false;
  }
  free(typeHolder);
  return true;
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

unsigned char* readBinaryPgm(char* fileName, int* width, int* height) {
  FILE* picture;
  unsigned char* pixelValues;
  int numberHolder;
  if (!(picture = fopen(fileName, "rb"))) {
    setColor(RED);
    printf("Couldn't read %s\nProbably doesn't exist\n", fileName);
    setColor(RESET);
    return NULL;
  }
  if (!isBinaryPgm(picture)) {
    setColor(RED);
    printf("%s is not a binary PGM file\n", fileName);
    setColor(RESET);
    fclose(picture);
    return NULL;
  }
  skipComments(picture);
  char* lineHolder = (char*) malloc(LINE_SIZE * sizeof(char));
  fscanf(picture, "%d %d", width, height);
  // get maximum value
  skipComments(picture);
  fscanf(picture, "%d", &numberHolder);
  fgetc(picture);
  free(lineHolder);
  pixelValues = (unsigned char*) malloc((*height) * (*width) * sizeof(unsigned char));
  if (!pixelValues) {
    setColor(RED);
    printf("%s:%d > Failed to allocate memory\n", __FILE__, __LINE__);
    setColor(RESET);
    return NULL;
  }
  fread(pixelValues, sizeof(unsigned char), (*width) * (*height), picture);
  fclose(picture);
  return pixelValues;
}

unsigned char* readAsciiPgm(char* fileName, int* width, int* height) {
  FILE* picture;
  unsigned char *pixelValues;
  int numberHolder, i = 0;
  if (!(picture = fopen(fileName, "rb"))) {
    setColor(RED);
    printf("Couldn't read %s\nProbably doesn't exist\n", fileName);
    setColor(RESET);
    return NULL;
  }
  if (!isAsciiPgm(picture)) {
    setColor(RED);
    printf("%s is not an ASCII PGM file\n", fileName);
    setColor(RESET);
    fclose(picture);
    return NULL;
  }
  skipComments(picture);
  fscanf(picture, "%d %d", width, height);
  // get maximum value
  skipComments(picture);
  fscanf(picture, "%d", &numberHolder);
  fgetc(picture);
  pixelValues = (unsigned char*) malloc((*height) * (*width) * sizeof(unsigned char));
  if (!pixelValues) {
    setColor(RED);
    printf("%s:%d > Failed to allocate memory\n", __FILE__, __LINE__);
    setColor(RESET);
    return NULL;
  }
  while (fscanf(picture, "%hhu ", &(pixelValues[i++])) == 1);
  fclose(picture);
  return pixelValues;
}

unsigned char* readPgm(char* fileName, int* width, int* height) {
  unsigned char* pixelValues = readBinaryPgm(fileName, width, height);
  if (!pixelValues) {
    setColor(YELLOW);
    printf("Trying ASCII PGM\n");
    setColor(RESET);
    pixelValues = readAsciiPgm(fileName, width, height);
  }
  return pixelValues;
}

int main(int argc, char const *argv[]) {
  if (argc < 3) {
    setColor(RED);
    printf("Error: Too few arguments\n");
    setColor(RESET);
    printf("Run as follows:\n./compare f1.pgm f2.pgm\n");
    return 1;
  }
  int i, w1, w2, h1, h2;
  unsigned char *pixelValues1, *pixelValues2;
  char *fileName = (char*) malloc(LINE_SIZE * sizeof(char));
  strcpy(fileName, argv[1]);
  pixelValues1 = readPgm(fileName, &w1, &h1);
  if (!pixelValues1) {
    free(fileName);
    return 1;
  }
  strcpy(fileName, argv[2]);
  pixelValues2 = readPgm(fileName, &w2, &h2);
  free(fileName);
  if (!pixelValues2) {
    free(pixelValues1);
    return 1;
  }
  if (w1!=w2 || h1!=h2) {
    printf("Dimensions not euqal: %dx%d!=%dx%d\n", w1,h1,w2,h2);
    free(pixelValues1),free(pixelValues2);
    return 1;
  }
  for (i = 0; i < w1*h1; i++) {
    if (pixelValues1[i] != pixelValues2[i]) {
      printf("At location (%d,%d) pixels are not euqal\n%u!=%u\n", (int) (i/w1), i % w1, pixelValues1[i], pixelValues2[i]);
      free(pixelValues1);
      free(pixelValues2);
      return 1;
    }
  }
  setColor(GREEN);
  printf("Match! '%s' and '%s' are exactly the same\n", argv[1], argv[2]);
  setColor(RESET);
  free(pixelValues1);
  free(pixelValues2);
  return 0;
}

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
