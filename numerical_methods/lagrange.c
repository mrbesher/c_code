#include <stdio.h>
#include <stdlib.h>
#define YELLOW  "\x1b[33m"
#define RESET   "\x1b[0m"
int main(int argc, char const *argv[]) {
  double points[2][200],l[200];
  int n,sameset,again;
  char answer;
  do {
    again=0;
    printf("This program will try to determine the output of an unknown function given a set of points.\nEnter the number of data points (x,y) you wish to enter: ");scanf("%d",&n);
    system("cls");system("clear");
    printf("Note that the function input is asked by the program as xn and the output as yn.\nWhere n is the index of the points.\n");
    for (size_t i = 0; i < n; i++) {
      printf("Enter x%ld: ",i+1);scanf("%lf",&points[0][i]);
      printf("Enter y%ld: ",i+1);scanf("%lf",&points[1][i]);
    }
    do {
      sameset=0;
      printf("Enter the value of the input you wish to determine the output at: ");scanf("%lf",&points[0][n]);
      for (size_t i = 0; i < n; i++) {
        l[i]=1;
        for (size_t j = 0; j < n; j++) {
          if (j!=i) {
            l[i]*=(points[0][n]-points[0][j])/(points[0][i]-points[0][j]);
          }
        }
      }
      points[1][n]=0;
      for (size_t i = 0; i < n; i++) {
        points[1][n]+=l[i]*points[1][i];
      }
      system("cls");system("clear");
      printf("When the input is x=%lf the output is y=%lf\n",points[0][n],points[1][n]);
      printf(YELLOW"Enter'e' to use a new set of points.\n"RESET"Do you wish to determine the output at the another point? (y/n/e): ");getchar();scanf("%c",&answer);
      if (answer=='y' || answer=='Y') {
        sameset=1;
      } else if (answer=='e' || answer == 'E') {
        again=1;
      }
      system("cls");system("clear");
    } while(sameset);
  } while(again);
  return 0;
}
