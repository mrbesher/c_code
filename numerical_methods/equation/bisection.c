#include <stdio.h>
#include <math.h>
int main(int argc, char const *argv[]) {
  int n,attempt=0;
  float a[100],start,end,epsilon,result_a=0,result_b,result,x,c,stop=0;
  printf("This program uses the bisection method to determine a root value for a given equation.\nGiven that the equation is expressed as a[1]*x^n+a[2]*x^(n-1)+....+a[n+1].\nEnter n: ");
  scanf("%d",&n);
  for (size_t i = 0; i < n+1; i++) {
    printf("Enter a[%ld]: ",i+1);scanf("%f",&a[i]);
  }
  printf("Enter a start point: ");scanf("%f",&start);
  printf("Enter an end point: ");scanf("%f",&end);
  printf("Enter the epsilon (i.e. max error accepted): ");scanf("%f",&epsilon);
  x=1;
  for (size_t i = 0; i <= n; i++) {
    result_a=result_a+x*a[n-i];
    x=x*start;
  }
  if (result_a>0) {
    c=start;
    start=end;
    end=c;
  }
  while (stop==0) {
    attempt++;
    printf("Trying(%d)...\n",attempt);
    result_a=0;result_b=0;result=0;
    x=1;
    for (size_t i = 0; i <= n; i++) {
      result_a=result_a+x*a[n-i];
      x=x*start;
    }
    x=1;
    for (size_t i = 0; i <= n; i++) {
      result_b=result_b+x*a[n-i];
      x=x*end;
    }
    if ((result_a<0 && result_b<0)||(result_a>0 && result_b>0)) {
      printf("result_a: %f and result_b: %f\nError: The entered range doesn't include a root.\nOR: The entered range starts and ends with results (i.e. y as in [x,y]) that have the same sign.\nLaunch the program again and enter a range that includes at least one root.\n",result_a,result_b);
      stop=1;
    } else if (result_a==0 || result_b==0) {
      stop=1;
      if (result_a==0) {
        c=start;
      } else  {
        c=end;
      }
    } else {
      c=(start+end)/2;
      x=1;
      for (size_t i = 0; i <= n; i++) {
        result=result+x*a[n-i];
        x=x*c;
      }
      if ((fabs(result)-0)<epsilon) {
        stop=1;
      } else if (result>0) {
        end=c;
      } else {
        start=c;
      }
    }
  }
  result=0;
  x=1;
  for (size_t i = 0; i <= n; i++) {
    result=result+x*a[n-i];
    x=x*c;
  }
  printf("if x=%f then y=%f\n",c,result);
  return 0;
}
