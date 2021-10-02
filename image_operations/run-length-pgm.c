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
#define EXPANDABLE_BLOCK 5

typedef struct bundle {
  uint16_t counter;
  unsigned char currentPixel;
} entity;

void help();
bool processInput(char*);
void setColor(int);
unsigned char* readPgm(char*, int* ,int*);
unsigned char* readAsciiPgm(char*, int* ,int*);
bool isBinaryPgm(FILE*);
bool isAsciiPgm(FILE*);
bool isSpace(char);
void skipComments(FILE*);
bool compressArrToFile(unsigned char*, int, int, char*, int);
bool decompressFile(char*, char*);
int isZpgm(FILE*);
bool passesControl(FILE*, int, int);
bool swapAllPixels(char*, int, int);
bool printZpgmHistogram(char*);
bool setPixelAtLoc(char*, int, int, unsigned char);
void addPixelInBlock(FILE*, entity*, entity, int, int*, int, unsigned char);
void compressFile(char*, char*);
void removeExtension(char*, char*);
bool isIntegerStr(char*);
size_t fwritePixel(const void*, size_t, size_t, FILE*);

int main(int argc, char const *argv[]) {
  char* inputStr = (char*) malloc(INPUT_SIZE * sizeof(char));
  printf("Welcome to Stipant\n");
  help();
  do {
    printf("> ");
    fgets(inputStr, INPUT_SIZE, stdin);
  } while(processInput(inputStr));
  free(inputStr);
  return 0;
}

void help() {
  printf(
          "Here are the commands you can use:\n"
          "help\t\t\t\t\t- prints available commands\n"
          "compress input.pgm [output.zpgm]\t- compresses the input file into output.zpgm\n"
          "decompress input.zpgm [output.pgm]\t- converts the encoded zpgm to pgm\n"
          "swapall input.zpgm prev next\t\t- sets all pixels with the value 'prev' to the value 'next'\n"
          "setpix input.zpgm row column value\t- sets pixel at specified location (0-indexed) to 'value'\n"
          "hist input.zpgm\t\t\t\t- prints the histogram of the enocded file input.zpgm\n"
          "exit\t\t\t\t\t- quits the program\n"
        );
}

bool processInput(char* inputStr) {
  int i, integer1, integer2;
  char* strPointer;
  bool shouldCont = true;
  char** arg = (char**) malloc(EXPANDABLE_BLOCK * sizeof(char*));
  for (i = 0; i < EXPANDABLE_BLOCK; i++) {
    arg[i] = (char*) malloc(LINE_SIZE * sizeof(char));
    strcpy(arg[i],"NULL");
  }
  i = 0;
  strPointer = strtok(inputStr, " ");
  strcpy(arg[i++], strPointer);
  while ((strPointer = strtok(NULL, " "))) {
    strcpy(arg[i++], strPointer);
  }
  strtok(arg[i -1], "\n");
  i=0;
  if (!strcmp(arg[i], "exit")) {
    shouldCont = false;
  } else if (!strcmp(arg[i], "help")) {
    help();
  } else if (!strcmp(arg[i], "compress")) {
    i++;
    if (strcmp(arg[i++], "NULL")) {
      if (strcmp(arg[i], "NULL")) {
        compressFile(arg[i-1], arg[i]);
      } else {
        removeExtension(arg[i-1], arg[i]);
        strcat(arg[i], "_encoded.zpgm");
        compressFile(arg[i-1], arg[i]);
      }
    } else {
      setColor(RED);
      printf("Error: no input provided\n");
      setColor(YELLOW);
      printf("Tip: type 'help' for usage\n");
      setColor(RESET);
    }
  } else if (!strcmp(arg[i], "decompress")) {
    i++;
    if (strcmp(arg[i++], "NULL")) {
      if (strcmp(arg[i], "NULL")) {
        decompressFile(arg[i-1], arg[i]);
      } else {
        removeExtension(arg[i-1], arg[i]);
        strcat(arg[i], "_decoded.pgm");
        decompressFile(arg[i-1], arg[i]);
      }
    } else {
      setColor(RED);
      printf("Error: no input provided\n");
      setColor(YELLOW);
      printf("Tip: type 'help' for usage\n");
      setColor(RESET);
    }
  } else if (!strcmp(arg[i], "swapall")) {
    i++;
    if (strcmp(arg[i], "NULL")) {
      if (isIntegerStr(arg[i+1]) && isIntegerStr(arg[i+2])) {
        integer1 = atoi(arg[i+1]), integer2 = atoi(arg[i+2]);
        if (integer1 <= 255 && integer2 <= 255) {
          swapAllPixels(arg[i], integer1, integer2);
        } else {
          setColor(RED);
          printf("Error: enter pixel value in range [0-255]\n");
          setColor(RESET);
        }
      } else {
        setColor(RED);
        printf("Error: please enter integers for pixel values\n");
        setColor(YELLOW);
        printf("Tip: type 'help' for usage\n");
        setColor(RESET);
      }
    } else {
      setColor(RED);
      printf("Error: no input provided\n");
      setColor(YELLOW);
      printf("Tip: type 'help' for usage\n");
      setColor(RESET);
    }
  } else if (!strcmp(arg[i], "setpix")) {
    i++;
    if (strcmp(arg[i], "NULL")) {
      if (isIntegerStr(arg[i+1]) && isIntegerStr(arg[i+2]) && isIntegerStr(arg[i+3])) {
        integer1 = atoi(arg[i+3]);
        if (integer1 <= 255) {
          setPixelAtLoc(arg[i], atoi(arg[i+1]), atoi(arg[i+2]), integer1);
        } else {
          setColor(RED);
          printf("Error: enter pixel value in range [0-255]\n");
          setColor(RESET);
        }
      } else {
        setColor(RED);
        printf("Error: please enter integers for location and pixel value\n");
        setColor(YELLOW);
        printf("Tip: type 'help' for usage\n");
        setColor(RESET);
      }
    } else {
      setColor(RED);
      printf("Error: no input provided\n");
      setColor(YELLOW);
      printf("Tip: type 'help' for usage\n");
      setColor(RESET);
    }
  } else if (!strcmp(arg[i], "hist")) {
    i++;
    if (strcmp(arg[i], "NULL")) {
      printZpgmHistogram(arg[i]);
    } else {
      setColor(RED);
      printf("Error: no input provided\n");
      setColor(YELLOW);
      printf("Tip: type 'help' for usage\n");
      setColor(RESET);
    }
  } else {
    setColor(YELLOW);
    printf("Cannot recognise command '%s'\n", arg[i]);
    printf("Tip: type 'help' for usage\n");
    setColor(RESET);
  }

  for (i = 0; i < EXPANDABLE_BLOCK; i++) {
    free(arg[i]);
  }
  free(arg);
  return shouldCont;
}

unsigned char* readPgm(char* fileName, int* width, int* height) {
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
  char* lineHolder = (char*) malloc(LINE_SIZE * sizeof(char));
  fscanf(picture, "%d %d", width, height);
  free(lineHolder);
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
  while (fscanf(picture, " %d ", &numberHolder) == 1) {
    if (numberHolder > 255 || numberHolder < 0) {
      setColor(RED);
      printf("Error: corrupt input. Some pixels are not in [0-255]\n");
      setColor(RESET);
      fclose(picture);
      return NULL;
    }
    pixelValues[i++] = numberHolder;
  };
  if (i != (*height) * (*width)) {
    setColor(RED);
    printf("Error: corrupt input. Read %d pixels, expected %d.\n", i, (*height) * (*width));
    setColor(RESET);
    fclose(picture);
    return NULL;
  }
  fclose(picture);
  return pixelValues;
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

bool isSpace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

bool compressArrToFile(unsigned char* pixelValues, int width, int height, char* fileName, int version) {
  FILE* picture;
  int i;
  if (!(picture = fopen(fileName, "wb"))) {
    setColor(RED);
    printf("Cannot create file (%s)\n", fileName);
    setColor(RESET);
    return false;
  }
  fprintf(picture, "ZP%d\n", version);
  fprintf(picture, "%d %d\n", width, height);
  entity instance;
  instance.counter = 1;
  instance.currentPixel = *(pixelValues);
  for (i = 1; i < width * height; i++) {
    if (*(pixelValues + i) != instance.currentPixel) {
      fwrite(&(instance.counter), sizeof(uint16_t), 1, picture);
      fwrite(&(instance.currentPixel), sizeof(unsigned char), 1, picture);
      instance.counter = 0, (instance.currentPixel) = *(pixelValues + i);
    }
    (instance.counter)++;
  }
  if (*(pixelValues + i - 1) == instance.currentPixel) {
    fwrite(&(instance.counter), sizeof(uint16_t), 1, picture);
    fwrite(&(instance.currentPixel), sizeof(unsigned char), 1, picture);
  }
  fclose(picture);
  return true;
}

bool decompressFile(char* compressedName, char* outputName) {
  FILE *compressed, *decompressed;
  int width, height, i, version;
  entity instance;
  size_t (*writepixPtr)(const void*, size_t, size_t, FILE*) = fwrite;
  if (!(compressed = fopen(compressedName, "rb"))) {
    setColor(RED);
    printf("Cannot open %s\n", compressedName);
    setColor(RESET);
    return false;
  }
  if (!(version = isZpgm(compressed))) {
    setColor(RED);
    printf("%s is not a zpgm file\n", compressedName);
    setColor(RESET);
    fclose(compressed);
    return false;
  }
  if (!(decompressed = fopen(outputName, "wb"))) {
    setColor(RED);
    printf("Cannot create %s\n", outputName);
    setColor(RESET);
    fclose(compressed);
    return false;
  }
  fprintf(decompressed, "P%d\n",version);
  if (version == 2) {
    writepixPtr = fwritePixel;
  }
  char* lineHolder = (char*) malloc(LINE_SIZE * sizeof(char));
  fgets(lineHolder, LINE_SIZE, compressed);
  sscanf(lineHolder, "%d %d", &width, &height);
  if (!passesControl(compressed, width, height)) {
    setColor(RED);
    printf("The file %s is corrupt\n", compressedName);
    setColor(RESET);
    free(lineHolder);
    fclose(compressed);
    fclose(decompressed);
    remove(outputName);
    return false;
  }
  fprintf(decompressed, "# decompressed by Stipant the squeezer\n");
  fprintf(decompressed, "%d %d\n", width, height);
  fprintf(decompressed, "255\n");
  while (fread(&(instance.counter), sizeof(uint16_t), 1, compressed) == 1) {
    fread(&(instance.currentPixel), sizeof(unsigned char), 1, compressed);
    for (i = 0; i < instance.counter; i++) {
      writepixPtr(&(instance.currentPixel), sizeof(unsigned char), 1, decompressed);
    }
  }
  free(lineHolder);
  fclose(compressed);
  fclose(decompressed);
  setColor(GREEN);
  printf("Wrote to output %s Successfully\n", outputName);
  setColor(RESET);
  return true;
}

int isZpgm(FILE* file) {
  char* typeHolder = (char*) malloc(LINE_SIZE * sizeof(char));
  fgets(typeHolder, LINE_SIZE, file);
  strtok(typeHolder, "\n");
  if (strcmp(typeHolder, "ZP5")) {
    if (strcmp(typeHolder, "ZP2")) {
      free(typeHolder);
      return 0;
    }
    free(typeHolder);
    return 2;
  }
  free(typeHolder);
  return 5;
}

size_t fwritePixel(const void *ptr, size_t size, size_t nmemb, FILE *picture) {
  fprintf(picture, "%u\n", *((unsigned char*)ptr));
  return 1;
}

bool passesControl(FILE* compressed, int width, int height) {
  int total = 0, location;
  entity instance;
  unsigned char prePixel;
  location = ftell(compressed);
  if (fread(&(instance.counter), sizeof(uint16_t), 1, compressed) != 1 || fread(&(instance.currentPixel), sizeof(unsigned char), 1, compressed) != 1) {
    printf("Empty File\n");
    return false;
  }
  prePixel = instance.currentPixel;
  total += instance.counter;
  while (fread(&(instance.counter), sizeof(uint16_t), 1, compressed) == 1) {
    fread(&(instance.currentPixel), sizeof(unsigned char), 1, compressed);
    if (prePixel == instance.currentPixel) {
      printf("corrupt RLE. Neighbor blocks with the same value\n");
      return false;
    }
    total += instance.counter;
    prePixel = instance.currentPixel;
  }
  if (total != width*height) {
    printf("insufficent pixel numbers\n");
    return false;
  }
  fseek(compressed, location, SEEK_SET);
  return true;
}

bool swapAllPixels(char* compressedName, int oldValue, int newValue) {
  FILE* compressed;
  int width, height, total = 0, size, i, version;
  if (!(compressed = fopen(compressedName, "rb"))) {
    setColor(RED);
    printf("Cannot open %s\n", compressedName);
    setColor(RESET);
    return false;
  }
  if (!(version = isZpgm(compressed))) {
    setColor(RED);
    printf("%s is not a zpgm file\n", compressedName);
    setColor(RESET);
    fclose(compressed);
    return false;
  }
  char* lineHolder = (char*) malloc(LINE_SIZE * sizeof(char));
  fgets(lineHolder, LINE_SIZE, compressed);
  sscanf(lineHolder, "%d %d", &width, &height);
  free(lineHolder);
  size = LINE_SIZE;
  entity* instanceArr = (entity*) malloc(size * sizeof(entity));
  entity instance;
  if (fread(&(instance.counter), sizeof(uint16_t), 1, compressed) != 1 || fread(&(instance.currentPixel), sizeof(unsigned char), 1, compressed) != 1) {
    setColor(RED);
    printf("Error: empty zpgm file\n");
    setColor(RESET);
    free(instanceArr);
    return false;
  }
  if (instance.currentPixel == oldValue) {
    instance.currentPixel = newValue;
  }
  instanceArr[total] = instance;
  total++;
  while (fread(&(instance.counter), sizeof(uint16_t), 1, compressed) == 1) {
    fread(&(instance.currentPixel), sizeof(unsigned char), 1, compressed);
    if (total >= size) {
      size *= 2;
      instanceArr = (entity*) realloc(instanceArr, size * sizeof(entity));
    }
    if (instance.currentPixel == oldValue) {
      instance.currentPixel = newValue;
    }
    if (instance.currentPixel == instanceArr[total - 1].currentPixel) {
      instanceArr[total - 1].counter += instance.counter;
    } else {
      instanceArr[total++] = instance;
    }
  }
  fclose(compressed);
  remove(compressedName);
  compressed = fopen(compressedName, "wb");
  fprintf(compressed, "ZP%d\n",version);
  fprintf(compressed, "%d %d\n", width, height);
  for (i = 0; i < total; i++) {
    fwrite(&(instanceArr[i].counter), sizeof(uint16_t), 1, compressed);
    fwrite(&(instanceArr[i].currentPixel), sizeof(unsigned char), 1, compressed);
  }
  free(instanceArr);
  fclose(compressed);
  setColor(GREEN);
  printf("Successfully swapped pixels of %d vlue with %d\n", oldValue, newValue);
  setColor(RESET);
  return false;
}

bool printZpgmHistogram(char* compressedName) {
  FILE* compressed;
  int width, height, i, total = 0, j=0;
  if (!(compressed = fopen(compressedName, "rb"))) {
    setColor(RED);
    printf("Cannot open %s\n", compressedName);
    setColor(RESET);
    return false;
  }
  if (!isZpgm(compressed)) {
    setColor(RED);
    printf("%s is not a zpgm file\n", compressedName);
    setColor(RESET);
    fclose(compressed);
    return false;
  }
  char* lineHolder = (char*) malloc(LINE_SIZE * sizeof(char));
  fgets(lineHolder, LINE_SIZE, compressed);
  sscanf(lineHolder, "%d %d", &width, &height);
  free(lineHolder);
  size_t *pixelCounter = (size_t*) malloc(256 * sizeof(size_t));
  memset(pixelCounter, 0, 256*sizeof(size_t));
  entity instance;
  while (fread(&(instance.counter), sizeof(uint16_t), 1, compressed) == 1) {
    fread(&(instance.currentPixel), sizeof(unsigned char), 1, compressed);
    pixelCounter[instance.currentPixel] += instance.counter;
    total++;
  }
  printf("VALUE\tCOUNT\n");
  for (i = 0; i < 256; i++) {
    if (pixelCounter[i]) {
      printf("%d\t%ld\n", i, pixelCounter[i]);
      j++;
    }
  }
  printf("Total unique colors: %d\nTotal blocks: %d\n", j, total);
  free(pixelCounter);
  fclose(compressed);
  return true;
}

void addPixelInBlock(FILE* compressed, entity* instanceArr, entity instance, int pixelCount, int* total, int loc, unsigned char value) {
  entity* instanceBlock = (entity*) malloc(EXPANDABLE_BLOCK * sizeof(entity));
  int blockSize = 0, offset = 0, i = 0;
  int inBlockLocation = loc - pixelCount;
  if (*total) {
    instanceBlock[blockSize++] = instanceArr[(*total) - 1];
    offset = -1;
  }
  if (!inBlockLocation) {
    instanceBlock[blockSize].counter = 1;
    instanceBlock[blockSize++].currentPixel = value;
    instanceBlock[blockSize] = instance;
    instanceBlock[blockSize++].counter--;
  } else if (inBlockLocation == instance.counter - 1) {
    instanceBlock[blockSize] = instance;
    instanceBlock[blockSize++].counter--;
    instanceBlock[blockSize].counter = 1;
    instanceBlock[blockSize++].currentPixel = value;
  } else {
    instanceBlock[blockSize].counter = inBlockLocation;
    instanceBlock[blockSize++].currentPixel = instance.currentPixel;
    instanceBlock[blockSize].counter = 1;
    instanceBlock[blockSize++].currentPixel = value;
    instanceBlock[blockSize].counter = instance.counter - 1 - inBlockLocation;
    instanceBlock[blockSize++].currentPixel = instance.currentPixel;
  }
  if (fread(&(instance.counter), sizeof(uint16_t), 1, compressed) == 1 && fread(&(instance.currentPixel), sizeof(unsigned char), 1, compressed) == 1) {
    instanceBlock[blockSize++] = instance;
  }
  (*total) += offset;
  while (!instanceBlock[i].counter)
    i++;
  instanceArr[(*total)++] = instanceBlock[i++];
  for (; i < blockSize; i++) {
    if (instanceArr[(*total)-1].currentPixel == instanceBlock[i].currentPixel) {
      instanceArr[(*total)-1].counter += instanceBlock[i].counter;
    } else if (instanceBlock[i].counter) {
      instanceArr[(*total)++] = instanceBlock[i];
    }
  }
  free(instanceBlock);
}

bool setPixelAtLoc(char* compressedName, int row, int column, unsigned char value) {
  FILE* compressed;
  int width, height, total = 0, size, i, pixelCount = 0, version;
  long int fileLocation;
  if (!(compressed = fopen(compressedName, "rb"))) {
    setColor(RED);
    printf("Cannot open %s\n", compressedName);
    setColor(RESET);
    return false;
  }
  if (!(version = isZpgm(compressed))) {
    setColor(RED);
    printf("%s is not a zpgm file\n", compressedName);
    setColor(RESET);
    fclose(compressed);
    return false;
  }
  char* lineHolder = (char*) malloc(LINE_SIZE * sizeof(char));
  fgets(lineHolder, LINE_SIZE, compressed);
  sscanf(lineHolder, "%d %d", &width, &height);
  free(lineHolder);
  if (column >= width || row >= height) {
    setColor(RED);
    printf("Error: The requested location (%d, %d) exceeds image size\nWidth: %d Height:%d\n", row, column, width, height);
    setColor(RESET);
    fclose(compressed);
    return false;
  }
  size = LINE_SIZE;
  entity* instanceArr = (entity*) malloc(size * sizeof(entity));
  entity instance;
  fileLocation = ftell(compressed);
  if (fread(&(instance.counter), sizeof(uint16_t), 1, compressed) != 1 || fread(&(instance.currentPixel), sizeof(unsigned char), 1, compressed) != 1) {
    setColor(RED);
    printf("Error: empty zpgm file\n");
    setColor(RESET);
    free(instanceArr);
    fclose(compressed);
    return false;
  }
  fseek(compressed, fileLocation, SEEK_SET);
  int loc = row * width + column;
  while (fread(&(instance.counter), sizeof(uint16_t), 1, compressed) == 1 &&
          fread(&(instance.currentPixel), sizeof(unsigned char), 1, compressed) == 1 &&
          pixelCount + instance.counter <= loc) {
    if (total >= size) {
      size *= 2;
      instanceArr = (entity*) realloc(instanceArr, size * sizeof(entity));
    }
    instanceArr[total++] = instance;
    pixelCount += instance.counter;
  }
  if (instance.currentPixel == value) {
    free(instanceArr);
    fclose(compressed);
    setColor(GREEN);
    printf("Successfully set the value of (%d, %d) to %d\n", row, column, value);
    setColor(RESET);
    return true;
  }
  if (total + EXPANDABLE_BLOCK >= size) {
    size *= 2;
    instanceArr = (entity*) realloc(instanceArr, size * sizeof(entity));
  }
  addPixelInBlock(compressed, instanceArr, instance, pixelCount, &total, loc, value);
  while (fread(&(instance.counter), sizeof(uint16_t), 1, compressed) == 1 &&
  fread(&(instance.currentPixel), sizeof(unsigned char), 1, compressed) == 1) {
    if (total >= size) {
      size *= 2;
      instanceArr = (entity*) realloc(instanceArr, size * sizeof(entity));
    }
    instanceArr[total++] = instance;
  }
  fclose(compressed);
  remove(compressedName);
  compressed = fopen(compressedName, "wb");
  fprintf(compressed, "ZP%d\n", version);
  fprintf(compressed, "%d %d\n", width, height);
  for (i = 0; i < total; i++) {
    fwrite(&(instanceArr[i].counter), sizeof(uint16_t), 1, compressed);
    fwrite(&(instanceArr[i].currentPixel), sizeof(unsigned char), 1, compressed);
  }
  free(instanceArr);
  fclose(compressed);
  setColor(GREEN);
  printf("Successfully set the value of (%d, %d) to %d\n", row, column, value);
  setColor(RESET);
  return true;
}

void removeExtension(char* filename, char* stripped) {
  strcpy(stripped, filename);
  char *end = stripped + strlen(stripped);
  while (end > stripped && *end != '.' && *end != '\\' && *end != '/') {
    end--;
  }
  if ((end > stripped && *end == '.') &&
      (*(end - 1) != '\\' && *(end - 1) != '/')) {
      *end = '\0';
  }
}

bool isIntegerStr(char* str) {
  int i = 0;
  while (str[i] && str[i] >= '0' && str[i] <= '9')
    i++;
  return str[i] == '\0';
}

void compressFile(char* inputFileName, char* outputFileName) {
  unsigned char* pixelValues;
  int width, height, version = 5;
  pixelValues = readPgm(inputFileName, &width, &height);
  if (!pixelValues) {
    setColor(YELLOW);
    printf("Trying ASCII PGM\n");
    setColor(RESET);
    version = 2;
    pixelValues = readAsciiPgm(inputFileName, &width, &height);
    if (!pixelValues)
      return;
  }
  if (compressArrToFile(pixelValues, width, height, outputFileName, version)) {
    setColor(GREEN);
    printf("Wrote data to '%s' Successfully\n", outputFileName);
    setColor(RESET);
  }
  free(pixelValues);

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
