#include<stdio.h>
#include<crypts_md5.h>
#include<random.h>
#include<time.h>
#include<stdlib.h>
#include<keccak.h>
#define N 100000

int main()
{
	mt19937 mt;
	mt_init(&mt, 100);
	unsigned int *dat = (unsigned int *)malloc(sizeof(unsigned int) * N);
	for(int i = 0; i < N; i++)
	{
		dat[i] = mt_gen(&mt);
	}
	bit256 a = keccak256(sizeof(unsigned int) * 8 * N, dat);
	print256(a);
	print128(md5(256, &a));
}