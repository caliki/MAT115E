#include <stdio.h>

int main()
{
	int k=1,p=1;
	while(k<10) 
	{
		p*=k;
		k+=2;
	}
	//for version
	p=1;
	for(k=0;k<10;k++)
	{
		p*=k;
	}
	return 0;
}
