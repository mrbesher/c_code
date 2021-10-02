#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define RESET   "\x1b[0m"
int lsquare(double points[][1000],int n, int order);
void gauss(double a[][100],int size);
int main(int argc, char const *argv[]) {
  int n, order;
  double points[2][1000];
  system("cls");system("clear");
  printf("This program tries to estimate a relationship between a dependent and an independent variable.\nEnter the number of points that you want to use: ");scanf("%d",&n);
  for (size_t i = 0; i < n; i++) {
    printf("Enter x%ld: ",i+1);scanf("%lf",&points[0][i]);
    printf("Enter y%ld: ",i+1);scanf("%lf",&points[1][i]);
  }
  do {
    system("cls");system("clear");
    printf("Enter the order of the equation you wish to get: ");scanf("%d",&order);
  } while(lsquare(points,n,order));
  return 0;
}
int lsquare(double points[][1000], int n, int order) {
  static double a[100][100], x_sum[100], b_sum[100],x0,y0;
  char answer;
  int resume;
  //get the sum of xi, xi^2 etc.
  for (size_t i = 1; i < 2*order+1; i++) {
    for (size_t j = 0; j < n; j++) {
      x_sum[i] += pow(points[0][j], i);
    }
  }
  //get the sum of yi, yi*xi, yi*xi^2 etc. save the sum to x_sum[order], x_sum[order+1]..
  for (size_t i = 0; i < order+1; i++) {
    for (size_t j = 0; j < n; j++) {
      b_sum[i] += points[1][j]*pow(points[0][j],i);
    }
  }

  //add x_sum[i+j] according to its coordinates
  for (size_t i = 0; i < order+1; i++) {
    for (size_t j = 0; j < order+1; j++) {
        a[j][i]=x_sum[j+i];
    }
  }
  a[0][0]=n;
  //add b_sum
  for (size_t i = 0; i < order+1; i++) {
    a[i][order+1]=b_sum[i];
  }
  gauss(a,order+1);
  system("cls");system("clear");
  do {
    resume=0;
    //print the function as a text
    printf("The approximated function is:\n"YELLOW"(%lf)",a[0][order+1]);
    for (size_t i = 0; i < order; i++) {
      printf("+(%lf)*x^%ld",a[i+1][order+1],i+1);
    }
    printf(RESET"\nDo you wish to evaluate the function at a value? (y/n): ");scanf(" %c",&answer);
    system("cls");system("clear");
    if (answer=='y' || answer=='Y') {
      resume=1;
      printf(YELLOW"Working on: (%lf)",a[0][order+1]);
      for (size_t i = 0; i < order; i++) {
        printf("+(%lf)*x^%ld",a[i+1][order+1],i+1);
      }
      printf("\n"RESET"Enter an x value to evaluate at: ");scanf("%lf",&x0);
      y0=0;system("cls");system("clear");
      for (size_t i = 0; i < order+1; i++) {
        y0+=a[i][order+1]*pow(x0,i);
      }
      printf(GREEN"x=%lf, y=%lf\n"RESET,x0,y0);
    }
  } while(resume);
  printf("Do you wish to get an approximated equation from another order? (y/n): ");scanf(" %c",&answer);
  if (answer=='y' || answer=='Y') {
    return 1;
  } else {
    return 0;
  }
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
