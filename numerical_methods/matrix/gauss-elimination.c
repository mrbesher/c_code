#include <stdio.h>
void gauss(double a[][100],int size);
int main(int argc, char const *argv[]) {
  int size;
  static double a[100][100];
  printf("Given the equations are represented as a[1]*X1+a[2]*X2+...+a[n]*Xn=C\nEnter the number of equations / variables: ");scanf("%d",&size);
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      printf("In the %ld.row: Enter the coefficient of x%ld: ",i+1,j+1);scanf("%lf",&a[i][j]);
    }
  printf("Enter the %ld. constant (c%ld): ",i+1,i+1);scanf("%lf",&a[i][size]);
  }
  printf("\n");
  gauss(a,size);
  for (size_t i = 0; i < size; i++) {
    printf("x%ld=%lf\n",i+1,a[i][size]);
  }
  return 0;
}
void gauss(double a[][100],int size) {
  double div;
  for (size_t i = 0; i < size; i++) {
    div=a[i][i];
    for (size_t j = i; j < size+1; j++) {
      a[i][j]/=div;
    }
    for (size_t j = i+1; j < size; j++) {
      div=a[j][i];
      for (size_t k = 0; k < size+1; k++) {
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
