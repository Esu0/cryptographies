#include<stdio.h>
#include"rsa.h"
#include<random.h>
#include<time.h>
#include<stdlib.h>

int main()
{
	unsigned int n = 1000;
	mt19937 mt;
	mt_init(&mt, (unsigned long long)time(NULL));
	unsigned long long *A = (unsigned long long *)malloc(sizeof(unsigned long long) * n);
	unsigned long long *B = (unsigned long long *)malloc(sizeof(unsigned long long) * n);
	unsigned long long *C = (unsigned long long *)malloc(sizeof(unsigned long long) * n * 2);
	unsigned long long *D = (unsigned long long *)malloc(sizeof(unsigned long long) * n * 2);
	clock_t s, e;
	int karatsuba_time, normal_time;
	for(unsigned int i = 0; i < n; i++)
	{
		A[i] = 0x00000000ffffffff & mt_gen(&mt);
		B[i] = 0x00000000ffffffff & mt_gen(&mt);
	}
	for(unsigned int i = 0; i < n * 2; i++)
	{
		C[i] = (D[i] = 0);
	}

	s = clock();
	_karatsuba_unsafe(C, A, B, n);
	e = clock();
	karatsuba_time = e - s;

	/*
	for(unsigned int i = n * 2; i > 0;)
	{
		i--;
		printf("%08llx", C[i]);
	}
	printf("\n");
	*/

	s = clock();
	big_mul_normal(D, A, B, n);
	e = clock();
	normal_time = e - s;

	/*
	for(unsigned int i = n * 2; i > 0;)
	{
		i--;
		printf("%08llx", D[i]);
	}
	printf("\n");
	*/

	int truth = 1;
	for(unsigned int i = n * 2; i > 0;)
	{
		i--;
		if(C[i] != D[i])
		{
			truth = 0;
			break;
		}
	}
	if(truth)printf("correct!\n");
	printf("karatsuba法:%dms\n", karatsuba_time);
	printf("筆算:%dms\n", normal_time);
	return 0;
}