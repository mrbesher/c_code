#include <stdio.h>
#include <math.h>
#include "tinyexpr.h"
int main(int argc, char const *argv[]) {
  double epsilon,x0,x1,y1,y0,x;
  char fx[100];
  te_variable vars[] = {"x",&x};
  printf("This program solves equations using the secant method.\nFor more: https://en.wikipedia.org/wiki/Secant_method\nNote: variables must be expressed as x. Otherwise no solution will be found.\nBefore starting, enter an epsilon value (i.e. max error accepted): ");
  scanf("%lf",&epsilon);
  printf("Enter the function:\n");scanf("%s",fx);
  te_expr *expr = te_compile(fx, vars, 1, 0);
  printf("Enter an x value where the range starts: ");scanf("%lf",&x0);
  printf("Enter an x value where the range ends: ");scanf("%lf",&x1);
  x=x0;
  if (te_eval(expr)>0) {
    x0=x1;
    x1=x;
  }
  x=x1;
  y1=te_eval(expr);
  x=x0;
  y0=te_eval(expr);
  if (1) { //(y0<0 && y1>0)||(y0>0 && y1<0)
    do {
      x=x1;
      y1=te_eval(expr);
      x=x0;
      y0=te_eval(expr);
      x0=x0-((x1-x0)/(y1-y0))*y0;
    } while(fabs(x-x0)>epsilon);
  } else
      printf("The entered range isn't applicable for this method.\n");
  printf("The root is: %lf\n",x0);
  return 0;
}
