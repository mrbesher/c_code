#include <stdio.h>
#include <math.h>
#include "tinyexpr.h"
int main(int argc, char const *argv[]) {
  double x0, x1,result,pre_result,epsilon,der,dx=1.0e-7;
  char fx[100];
  te_variable vars[] = {"x",&x0};
  printf("This program approxamtes a root value to a given function using Newton-Raphson method.\nFor more: https://en.wikipedia.org/wiki/Newton's_method\nNote: variables must be expressed as x. Otherwise no solution will be found.\nBefore starting, enter an epsilon value (i.e. max error accepted): ");
  scanf("%lf",&epsilon);
  printf("Enter the function:\n");scanf("%s",fx);
  printf("Enter an x value to start from: ");scanf("%lf",&x1);
  te_expr *expr = te_compile(fx, vars, 1, 0);
  do {
    x0=x1;
    //determine the derivative
    x0-=dx;
    pre_result=te_eval(expr);
    x0+=2*dx;
    result=te_eval(expr);
    der=(result-pre_result)/(2*dx);
    x0-=dx;
    result=te_eval(expr);
    x1=x0-(result/der); //determine x1
  } while(fabs(x1-x0)>epsilon);
  te_free(expr);
  printf("The root is: %lf\n",x1);
  return 0;
}
