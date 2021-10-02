#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int equalizeHistogram(int pixelValues[][500], int width, int height);
void readValuesFromPgm(int pixelValues[][500], int *width, int *height);
void writeValuesToPgm(int pixelValues[][500], int width, int height);
void visualizeTimeComplexity(int pixelValues[][500]);
int main(int argc, char const *argv[]) {
  int pixelValues[500][500], width, height;
  char operator;
  printf("Enter '1' to apply the algorithm to a photo (.pgm) and '2' to see a visualization of its time complexity.\n(1-2): ");scanf("%c", &operator);
  switch (operator) {
    case '1':
    readValuesFromPgm(pixelValues, &width, &height);
    equalizeHistogram(pixelValues, width, height);
    writeValuesToPgm(pixelValues, width, height);
    break;
    case '2':
    //print out something that shows time complexity
    visualizeTimeComplexity(pixelValues);
  }
  return 0;
}

void readValuesFromPgm(int pixelValues[][500], int *width, int *height) {
  char answeString[100];
  printf("Enter file name: "); scanf(" %s",answeString);
  FILE *picture;
  picture = fopen(answeString, "r");
  //scan metadata about the photo
  int i,j,temp;
  fscanf(picture, "%d", width);
  fscanf(picture, "%d", height);
  fscanf(picture, "%d", &temp);
  //write file values into 2d array
  j=0;
  do {
    for (size_t i = 0; i < *width; i++) {
      fscanf(picture, "%d", &pixelValues[j][i]);
    }
    j++;
  } while((temp = fgetc(picture)) != EOF);
  fclose(picture);
}

int equalizeHistogram(int pixelValues[][500], int width, int height) {
  static double pixelCounter[256], cumulativeFreq[256];
  int pixelsNumber, iteration=0;
  //matris içindeki değerleri tarayıp ilgili piksel değeri için pixelCounter elemanlarını artır
  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      pixelCounter[pixelValues[i][j]]++;
    }
  }
  //pixelCounter dizisindeki elemanlari toplam piksel sayisina bolerek normalize et
  pixelsNumber = width*height;
  for (size_t i = 0; i < 256; i++) {
    pixelCounter[i] /= pixelsNumber;
    //printf("%f\n",pixelCounter[i]);
  }
  //kumulatif frekans tablosu olustur
  cumulativeFreq[0] = pixelCounter[0];
  for (size_t i = 1; i < 256; i++) {
    cumulativeFreq[i] = cumulativeFreq[i-1] + pixelCounter[i];
  }
  //hitogram esitlemesini uygula [piksel degerlerini ilgili kumulatif frekanslarini 255 ile carparak degistir]
  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      pixelValues[i][j] = (int)(cumulativeFreq[pixelValues[i][j]]*255+0.5);
      iteration++;
    }
  }
  return iteration;
}

void writeValuesToPgm(int pixelValues[][500], int width, int height) {
    char answeString[100];
    printf("Enter file name to output (.pgm): "); scanf(" %s",answeString);
    FILE *pgmPicture;
    pgmPicture = fopen(answeString, "w");
    //PGM uzantisi ile ilgili boyut ve tur bilgisi bilgi icin: https://en.wikipedia.org/wiki/Netpbm
    fprintf(pgmPicture, "P2\n");
    fprintf(pgmPicture, "%d %d\n", width, height);
    fprintf(pgmPicture, "255\n");
    for (size_t i = 0; i < height; i++) {
      for (size_t j = 0; j < width; j++) {
        fprintf(pgmPicture, "%d\n", pixelValues[i][j]);
      }
    }
    fclose(pgmPicture);
}

void visualizeTimeComplexity(int pixelValues[][500]) {
  //N sayisi (dataSize)^2 olacaktir cunku fotografin boyutlari ayni
  int dataSize = 10, testNumber = 20, maxIteration = 0, iterationHistogram[20];
  static int visualMatrix[20][20];
  srand(time(0));
  for (size_t i = 0; i < testNumber; i++) {
    //pixelValues matrisine rastgele degerler ata
    for (size_t j = 0; j < dataSize; j++) {
      pixelValues[0][j] = rand()%256;
    }
    iterationHistogram[i] = equalizeHistogram(pixelValues, dataSize, 1);
    dataSize += 10;
  }
  //iterasyon sayilarini ekrana yazdir
  printf("N (Data size)\t");
  for (size_t i = 0; i < testNumber; i++) {
    printf("%ld\t",10+i*10);
  }
  printf("\nIterations\t");
  for (size_t i = 0; i < testNumber; i++) {
    printf("%d\t",iterationHistogram[i]);
  }
  printf("\n");
  //iterasyon histogrami dizisinindeki max degeri bul sonra 0-20 arasinda normalize et
  for (size_t i = 0; i < testNumber; i++) {
    if (iterationHistogram[i] > maxIteration) {
      maxIteration = iterationHistogram[i];
    }
  }
  for (size_t i = 0; i < testNumber; i++) {
    iterationHistogram[i] = (int)((iterationHistogram[i]*20)/maxIteration+0.5);
  }
  printf("\n");
  for (size_t i = 0; i < testNumber; i++) {
    //histogramdaki degere gore matrisin ilgili elemaninina 1 ata
    visualMatrix[20-iterationHistogram[i]][i]=1;
  }
  for (size_t i = 0; i < 20; i++) {
    for (size_t j = 0; j < testNumber; j++) {
      if (visualMatrix[i][j]) {
        printf("* ");
      } else {
        printf("  ");
      }
    }
    printf("\n");
  }
}
