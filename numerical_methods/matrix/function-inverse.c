#include <stdio.h>
void inverse(double a[][100],int size);
int main(int argc, char const *argv[]) {
  int size;
  static double a[100][100];
  printf("This program returns the inverse of any given invertible matrix.\nEnter the size of the matrix (nxn):");scanf("%d",&size);
  for (size_t i = 0; i < size; i++) {
    a[i][size+i]=1;
  }
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      printf("a[%ld][%ld]: ",i+1,j+1);scanf("%lf",&a[i][j]);
    }
  }
  printf("\n");
  inverse(a,size);
  for (size_t i = 0; i < size; i++) {
    for (size_t j = size; j < 2*size; j++) {
      printf("%4.4lf\t",a[i][j]);
    }
    printf("\n");
  }
  return 0;
}
void inverse(double a[][100],int size) {
  double div;
  for (size_t i = 0; i < size; i++) {
    div=a[i][i];
    for (size_t j = i; j < size*2; j++) {
      a[i][j]/=div;
    }
    for (size_t j = i+1; j < size; j++) {
      div=a[j][i];
      for (size_t k = 0; k < size*2; k++) {
        a[j][k]-=div*a[i][k];
      }
    }
  }
  for (size_t i = 1; i < size; i++) {
    for (size_t j = i; j < size; j++) {
      div=a[size-j-1][size-i];
      a[size-j-1][size-i]-=div*a[size-i][size-i];
      for (size_t k = 0; k < size; k++) {
        a[size-j-1][size+k]-=div*a[size-i][size+k];
      }
    }
  }
}
