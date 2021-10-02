#include <stdio.h>
#include <math.h>
void diagonal(double a[][100],int size);
void seidel(double a[][100],int size,double x0[][100],double epsilon);
int main(int argc, char const *argv[]) {
  int size;
  static double a[100][100],x0[2][100],epsilon;
  printf("Before starting, enter an epsilon value (i.e. max error accepted): ");scanf("%lf",&epsilon);
  printf("Enter the number of equations / variables: ");scanf("%d",&size);
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      printf("In the %ld.row: Enter the coefficient of x%ld: ",i+1,j+1);scanf("%lf",&a[i][j]);
    }
  printf("Enter the %ld. constant (c%ld): ",i+1,i+1);scanf("%lf",&a[i][size]);
  }
  printf("\n");
  printf("Enter the starting value for:\n");
  for (size_t i = 0; i < size; i++) {
    printf("x%ld: ",i+1);scanf("%lf",&x0[0][i]);
  }
  diagonal(a,size);
  seidel(a,size,x0,epsilon);
  for (size_t i = 0; i < size; i++) {
    printf("x%ld=%lf\n",i+1,x0[1][i]);
  }
  return 0;
}
void diagonal(double a[][100],int size) {
  int t,k,counter,elements=0;
  static double max[3][1000],rows[100],copy[100][100];
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      counter=0;
      while (fabs(a[i][j])>max[0][elements-counter] && counter<=elements) {
        max[0][elements-counter+1]=max[0][elements-counter];
        max[1][elements-counter+1]=max[1][elements-counter];
        max[2][elements-counter+1]=max[2][elements-counter];
        counter++;
      }
        max[0][elements-counter+1]=fabs(a[i][j]);
        max[1][elements-counter+1]=i;
        max[2][elements-counter+1]=j;
      elements++;
    }
  }
  for (size_t i = 0; i < size; i++) {
    rows[i]=-1;
  }
  counter=0;k=0;
  while (counter<size) {
    if (rows[(int)max[1][k]]==-1) {
      t=0;
      while (max[2][k]!=rows[t] && t<size) {
        t++;
      }
      if (t==size) {
        rows[(int)max[1][k]]=max[2][k];
        counter++;
      }
    }
    k++;
  }
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size+1; j++) {
      copy[i][j]=a[i][j];
    }
  }
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size+1; j++) {
      a[(int)rows[i]][j]=copy[i][j];
    }
  }
}
void seidel(double a[][100],int size,double x0[][100],double epsilon) {
  int positive=0;
  static double swap;
  while (positive<size) {
    for (size_t i = 0; i < size; i++) {
      x0[1][i]=a[i][size]+x0[0][i]*a[i][i];
      for (size_t j = 0; j < size; j++) {
        x0[1][i]-=a[i][j]*x0[0][j];
      }
      x0[1][i]/=a[i][i];
      swap=x0[0][i];
      x0[0][i]=x0[1][i];
      x0[1][i]=swap;
    }
    positive=0;
    for (size_t i = 0; i < size; i++) {
      if (fabs(x0[1][i]-x0[0][i])<epsilon) {
        positive++;
      }
    }
    for (size_t i = 0; i < size; i++) {
      x0[1][i]=x0[0][i];
    }
  }
}
