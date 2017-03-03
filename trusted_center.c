#include <stdio.h>
#include "numbers/numbers.h"
int main()
{
	prime_init();
	int p1, p2;
	p1 = GetLargePrimeNumber();
	sleep(2);
	p2 = GetLargePrimeNumber();

	FILE *fp;
	fp = fopen("trust.txt", "w");
	if (fp==NULL)
	{
		printf("can't open file\n");
		return;
	}
	int ad = p1*p2;
	printf("%d - %d\t%d\n",p1, p2, ad );
	
	fprintf(fp, "%d\n", ad);
	fclose(fp);
	return 0;
}