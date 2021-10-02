#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tinyexpr.h"
int main(int argc, char const *argv[]) {
  double dx=1.0e-7,x0,h_result, h_preresult,der_h,epsilon;
  char hx[100];
  te_variable vars[] = {"x",&x0};
  printf("This program finds a root of a given function using the fixed-point iteration method.\nFore more: https://en.wikipedia.org/wiki/Fixed-point_iteration\nNote: variables must be expressed as x. Otherwise no solution will be found.\n");
  printf("Before starting, enter an epsilon (i.e. max error accepted): ");scanf("%lf",&epsilon);
  do {
    printf("Given the function is expressed as g(x)=h(x) where g(x)=x Enter h(x):\n");scanf("%s",hx);
    te_expr *expr = te_compile(hx, vars, 1, 0);
    printf("Enter an x value to start from: ");scanf("%lf",&x0);
    h_result=te_eval(expr);
    x0+=dx;
    h_preresult=te_eval(expr);
    te_free(expr);
    x0-=dx;
    der_h=fabs((h_result-h_preresult)/dx);
    printf("der: %lf\n",der_h);
    if (der_h>1) {
      printf("\033[31mError: Can't use Fixed-point iteration on this form.\nTry writing the expression in another way or choose a diffrent starting value.\033[0m\n");
    }
  } while(der_h>1);
  while (fabs(x0-h_result)>epsilon) {
    x0=h_result;
    te_expr *expr = te_compile(hx, vars, 1, 0);
    h_result=te_eval(expr);
    te_free(expr);
  }
  printf("\033[32mThe root is: %lf\033[0m\n",h_result);
  return 0;
}
