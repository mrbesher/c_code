#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tinyexpr.h"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define RESET   "\x1b[0m"
#define METHODS 3
double derivative(double x0,int method,char expression[][100]);
double trapez(double a, double b, char expression[][100], int change);
double simpson(double a, double b, char expression[][100], int change);
int main(int argc, char const *argv[]) {
  double x0,a,b;
  char operator, expression[50][100];
  system("cls");system("clear");
  printf("This program includes several derivative / integration finding methods.\n");
  do {
    printf("Enter the expression you wish to evaluate below (note that the variable must be expressed as x):\n");
    scanf("%s",expression[0]);
    do {
      system("cls");system("clear");printf("Working on: %s\n",expression[0]);
      printf( YELLOW"Enter 'e' to rewrite the expression\n"RESET
              "1- Derivative using finite differecnce   - https://en.wikipedia.org/wiki/Finite_difference\n"
              "2- Trapezoidal integration               - https://en.wikipedia.org/wiki/Trapezoidal_rule\n"
              "3- Integration using Simpson's rule      - https://en.wikipedia.org/wiki/Simpson's_rule\n"
              "Choose one of the methods above (1-%d): ",METHODS);scanf(" %c",&operator);
      system("cls");system("clear");printf("Working on: %s\n",expression[0]);
      switch (operator) {
        case '1':
          printf("Please choose a differecnce method:\n(1)central / (2)forward / (3)backward (1-3): ");scanf(" %c",&operator);
          printf("Enter a point to evaluate at (x): ");scanf("%lf",&x0);
          if ((operator-'0')>0 && (operator-'0')<4) {
            printf(GREEN"%lf\n"RESET,derivative(x0, operator-'0', expression));
          } else printf(RED"Error: option unavailable.\n"RESET"Exiting method...\n");
          break;
        case '2':
          printf("Given that the defined integral is over the interval (a,b).\n");
          printf("Enter a: ");scanf("%lf",&a);
          printf("Enter b: ");scanf("%lf",&b);
          printf("h (the change in x) is defined as 1.0e-4. Do you wish to change it? (y/n): ");getchar();scanf(" %c",&operator);
          if (operator=='y')
            printf(GREEN"%lf\n"RESET,trapez(a,b,expression,1));
          else
            printf(GREEN"%lf\n"RESET,trapez(a,b,expression,0));
          break;
        case '3':
          printf("Given that the defined integral is over the interval (a,b).\n");
          printf("Enter a: ");scanf("%lf",&a);
          printf("Enter b: ");scanf("%lf",&b);
          printf("n (the number of areas) is set to %1.0lf. Do you wish to change it? (y/n): ",fabs(b-a)*2);getchar();scanf(" %c",&operator);
          if (operator=='y')
            printf(GREEN"%lf\n"RESET,simpson(a,b,expression,1));
          else
            printf(GREEN"%lf\n"RESET,simpson(a,b,expression,0));
          break;
        case 'e':
          operator = 126;
          break;
        default:
        printf(RED"Error: input doesn't lead to any method.\n"RESET);
      }
      printf("Do you wish to apply another method on the expression? (y/n):");if(operator!=126)scanf(" %c",&operator);
      printf("\n");system("cls");system("clear");
    } while(!(operator - 'y') || !(operator - 'Y'));} while (operator==126);
  return 0;
}
double derivative(double x0,int method,char expression[][100]) {
  te_variable vars[] = {"x",&x0};
  double dx=1.0e-6, f_1, f_0,der;
  te_expr *expr = te_compile(expression[0], vars, 1, 0);
  switch (method) {
    case 1:
      x0 += dx;
      f_1 = te_eval(expr);
      x0 -= 2*dx;
      f_0 = te_eval(expr);
      der = (f_1-f_0)/(2*dx);
      break;
    case 2:
      f_0 = te_eval(expr);
      x0 += dx;
      f_1 = te_eval(expr);
      der = (f_1-f_0)/dx;
      break;
    case 3:
      f_1 = te_eval(expr);
      x0 -= dx;
      f_0 = te_eval(expr);
      der = (f_1-f_0)/dx;
      break;
  }
  te_free(expr);
  return der;
}
double trapez(double a, double b, char expression[][100], int change) {
  double swap, h=1.0e-4,n,a0,y1,y0,sum=0;
  te_variable vars[] = {"x",&a0};
  te_expr *expr = te_compile(expression[0], vars, 1, 0);
  if (a>b) {
    swap = b;
    b=a;
    a=swap;
  }
  if (change) {
    printf("Enter the number of trapezoids you wish to be used: ");scanf("%lf",&n);
    h = (b-a)/n;
  }
  a0=a;
  while (a0<b) {
    y0=te_eval(expr);
    a0 += h;
    y1=te_eval(expr);
    sum+=(h/2)*(y0+y1);
  }
  te_free(expr);
  return sum;
}
double simpson(double a, double b, char expression[][100], int change) {
  double swap, h=0.5,n,a0,sum=0;
  te_variable vars[] = {"x",&a0};
  te_expr *expr = te_compile(expression[0], vars, 1, 0);
  if (a>b) {
    swap = b;
    b=a;
    a=swap;
  }
  if (change) {
    do {
      printf("Enter the number of areas you wish to be used: ");scanf("%lf",&n);
      if (fmod(n,2)!=0) {
        printf(RED"Error: n must be even\n"RESET);
      }
    } while(fmod(n,2)!=0);
    h = (b-a)/n;
  }
  n=(b-a)/h;
  a0=a;
  sum += te_eval(expr);
  a0+=h;
  while (a0<b) {
      sum+=4*te_eval(expr);
      a0+=2*h;
  }
  a0=a+2*h;
  while (a0<b) {
      sum+=2*te_eval(expr);
      a0+=2*h;
  }
  sum += te_eval(expr);
  sum *= (h/3);
  return sum;
}
