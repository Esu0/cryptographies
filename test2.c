#include<stdio.h>
typedef union _bit256
{
	unsigned long long words[4];
	unsigned char bytes[32];
} bit256;

int main()
{
	bit256 a;
	a.words[0] = 4;
	a.words[1] = 3;
	a.words[2] = 2;
	a.words[3] = 100;
	for(int i = 32; i > 0;)
	{
		i--;
		printf("%02x", a.bytes[i]);
	}
	printf("\n");
	unsigned int b = 0x12345678ul;
	unsigned char *c = (unsigned char *)(&b);
	printf("%02x\n", *c);
}