#include <stdio.h>
int main(int argc, char const *argv[]) {
  int size,start=0;
  static double a[100][100],copy[100][100];
  printf("This program returns the inverse of any given invertible matrix.\nEnter the size of the matrix (nxn):");scanf("%d",&size);
  for (size_t i = 0; i < size; i++) {
    a[i][size+i]=1;
  }
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      printf("\na[%ld][%ld]: ",i+1,j+1);scanf("%lf",&a[i][j]);
    }
  }
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < 2*size; j++)
      copy[i][j]=a[i][j];
  }
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < 2*size; j++) {
      printf("%4.4lf\t",a[i][j]);
    }
    printf("\n");
  }
  for (size_t i = 0; i < size; i++) { //loop for the whole matrix
    for (size_t j = i; j < 2*size; j++) {
      a[i][j]/=a[i][i];
      for (size_t k = i+1; k < size; k++) {
        printf("a[%ld][%ld](%1.1lf)-=copy[%ld][%ld](%1.1lf)*a[%ld][%ld](%1.1lf)\n",k,j,a[k][j],k,i,copy[k][i],k-1,j,a[k-1][j]);
        a[k][j]-=copy[k][i]*a[k-1][j];
      }
    }
  }
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < 2*size; j++) {
      printf("%4.4lf\t",a[i][j]);
    }
    printf("\n");
  }
  return 0;
}
