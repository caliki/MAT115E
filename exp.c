#include<stdio.h>
#include<math.h>

void coolfunction(int n, double* result)
{
    *result = exp(n);
};
int main() {
   int n;
   double result=0;
   printf("Enter an integer:");
   scanf("%i",&n);
   coolfunction(n,&result);
   printf("\n e^ %i = %lf",n,result);
}
