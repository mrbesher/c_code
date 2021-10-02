#include <stdio.h>
#include <math.h>
int main(int argc, char const *argv[]) {
  int n,attempt=0,stop=0;
  float a[50],x0,x,delta,epsilon,pre_result=0,result=0;
  printf("This program determines an approximate solution for linear equations using graphical method.\nGiven that the equation is expressed as a[1]*x^n+a[2]*x^(n-1)+....+a[n+1].\nEnter n: ");
  scanf("%d",&n);
  for (size_t i = 0; i < n+1; i++) {
    printf("Enter a[%ld]:",i+1);scanf("%f",&a[i]);
  }
  printf("Enter a start point: ");scanf("%f",&x0);
  printf("Enter a delta: ");scanf("%f",&delta);
  printf("Enter the epsilon (i.e. max error accepted): ");scanf("%f",&epsilon);
  x=1;
  for (size_t i = 0; i < n+1; i++) {
    pre_result=pre_result+x*a[n-i];
    x=x*x0;
  }
  while (stop==0 && attempt<90) {
    x0+=delta;
    x=1;
    result=0;
    for (size_t i = 0; i < n+1; i++) {
      result=result+x*a[n-i];
      x=x*x0;
    }
    if (((pre_result<0 && result>0) || (pre_result>0 && result<0)) || result==0.0000) {
      if ((result-pre_result)<0 && (pre_result-result)<epsilon) {
        stop=1;
      } else if (fabs(result-pre_result)<epsilon) {
        stop=1;
      } else if (result==0.00000) {
        stop=1;
      } else {
        x0=x0-delta;
        delta=delta/2;
        x=1;
        result=0;
        for (size_t i = 0; i < n+1; i++) {
          result=result+x*a[n-i];
          x=x*x0;
        }
        printf("entered delta/2\n");
      }
    }
    pre_result=result;
    attempt++;
    printf("trying... (%d). x=%f Result=%f\n",attempt,x0,result);
  }
  printf("when x is %f the result would be %f\n",x0,result);
  return 0;
}
