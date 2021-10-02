#include <stdio.h>
#include <math.h>
#include "tinyexpr.h"
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
void nonlinear_twovar(double a[][100], double x0[][100], double epsilon, char expression[][100]);
int main(int argc, char const *argv[]) {
  static double epsilon, x0[2][100], a[100][100];
  char expression[50][100];
  printf("Before starting, enter an epsilon value (i.e. max error accepted): ");scanf("%lf",&epsilon);
  printf("Given the equations are expressed as: a*x^n+b*y^m+...+C=0\nNote that variables MUST be expressed as x and y\nEnter the first equation (variables: x,y): ");scanf("%s",expression[0]);
  printf("Enter the second equation (variables: x,y): ");scanf("%s",expression[1]);
  printf("Enter a starting point for x: ");scanf("%lf",&x0[0][0]);
  printf("Enter a starting point for y: ");scanf("%lf",&x0[0][1]);
  nonlinear_twovar(a,x0,epsilon,expression);
  return 0;
}
void nonlinear_twovar(double a[][100], double x0[][100], double epsilon, char expression[][100]) {
  te_variable vars[] = {{"x",&x0[0][0]},{"y",&x0[0][1]}};
  double dx=1.0e-7, f_1, f_0, der;
  do {
    for (size_t i = 0; i < 2; i++) {
      te_expr *expr = te_compile(expression[i], vars, 2, 0);
      x0[0][0] += dx;
      f_1 = te_eval(expr);
      x0[0][0] -= 2*dx;
      f_0 = te_eval(expr);
      x0[0][0] += dx;
      a[i][0] = (f_1-f_0)/(dx*2); //write d(fx)/dx to a[0][0]
      x0[0][1] += dx;
      f_1 = te_eval(expr);
      x0[0][1] -= 2*dx;
      f_0 = te_eval(expr);
      x0[0][1] += dx;
      a[i][1] = (f_1-f_0)/(dx*2); //write d(fx)/dy to a[0][1]
      a[i][2] = -te_eval(expr); //write fx to a[0][2]
    }
    gauss(a, 2);
    x0[0][0] += a[0][2];
    x0[0][1] += a[1][2];
  } while(fabs(a[0][2])>epsilon && fabs(a[1][2])>epsilon);
}
