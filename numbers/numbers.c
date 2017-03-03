#include "numbers.h"
struct prime_numbers
{
	int primeNumbers[2048];
	int size;	
} pr_num;

int GetRandomNumber(const int min, const int max)
{
	srand(time(NULL));
	return (rand() % (max - min)) + min;
}
static void GeneratePrimeNumbers(int min, int max)
{
	int k = 0;
	int j, i;
	pr_num.size = 0;
	for (i = min; i != max + 1; i++) {
		j;
		for (j = 2; j < i; j++) {
			if (i % j == 0) {
				break;
			}
		}
		if (i == j) {
			pr_num.primeNumbers[pr_num.size++] = i;
		}
	}
}


void prime_init()
{
	GeneratePrimeNumbers(1, 100);
}

static int getGCD(int a, int b)	////Greatest Common Divisor
{
	int c;

	while (a != 0) {
		c = a;
		a = b % a;
		b = c;
	}
	return b;
}

int getCoprime(int n)
{
	int coprime;
	
	do {
		coprime = GetRandomNumber(2, n);
	}
	while (getGCD(n, coprime) != 1);
	return coprime;
	
}

int GetRandomSign()
{
	return (rand() % 2) ? -1 : 1;
}



int GetLargePrimeNumber()
{
	int index = GetRandomNumber(0, pr_num.size);
	return pr_num.primeNumbers[index];
}
