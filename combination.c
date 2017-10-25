#include<stdio.h>

int factorial(int m)
{
    int i,result=1;
    for(i = 2;i<=m;i++)
    {
        result *= i;
    }
    return result;
}

int comb(int n,int r)
{
    return factorial(n)/(factorial(r)*factorial(n-r));
}

int main() {
   int m,i,j;
   scanf("%i",&m);
   for(i=m-1;i>=0;i--)
   {
       for(j=0;j<=i;j++)
       {
           printf("%i ", comb(i,j));
       }
       printf("\n");
   }
   return 0;
}
