//SORU 751
//PROGRAM C

#include <stdio.h>
int n,m,l;
main()
{
	freopen("polly.in","r",stdin);
	freopen("polly.out","w",stdout);
	int t=0,k,m=0;
	scanf("%d",&n);
	while(m<n)
	{
		if(m!=0)
			printf("\n");
		l=0;
		t=0;
		while(l!='\n')
		{
			scanf("%d%c",&k,&l);
			t=t+k;
		}
		printf("%d YARDS",t);
		m++;
	}
}
