#include"crypts.h"
#include<stdio.h>
#include<stdlib.h>
const int p = 103, q = 53, n = 5459, e = 433, d = 49;

//4294967295進数の多倍長
typedef struct __biguint
{
	unsigned long long *digits;
	unsigned int size;
	unsigned int capacity;
	unsigned long long max_value;
} biguint;

unsigned int max2pow(unsigned int x)
{
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x + 1;
}

biguint biguint_new(unsigned int size)
{
	biguint new_inst;
	unsigned int cap = max2pow(size);
	unsigned long long *p = (unsigned long long *)malloc(sizeof(unsigned long long) * cap);
	if(p == NULL)
	{
		new_inst.digits = NULL;
		new_inst.size = 0;
		new_inst.capacity = 0;
		new_inst.max_value = 0;
		return new_inst;
	}
	for(unsigned int i = 0; i < size; i++)
		p[size] = 0;
	new_inst.digits = p;
	new_inst.size = size;
	new_inst.capacity = cap;
	new_inst.max_value = UINT_MAX;
	return new_inst;
}

void biguint_delete(biguint *integer)
{
	if(integer->digits != NULL)
	{
		free(integer->digits);
	}
}

biguint biguint_with_integer(unsigned long long a)
{
	biguint new_inst;
	unsigned long long *p = (unsigned long long *)malloc(sizeof(unsigned long long) * 2);
	if(p == NULL)
	{
		new_inst.digits = NULL;
		new_inst.size = 0;
		new_inst.capacity = 0;
		new_inst.max_value = 0;
		return new_inst;
	}
	p[0] = (unsigned int)(0x00000000ffffffffull & a);
	p[1] = (unsigned int)(a >> 32);
	new_inst.digits = p;
	new_inst.size = 2;
	new_inst.capacity = 2;
	new_inst.max_value = UINT_MAX;
	return new_inst;
}

static void _extend1digit(biguint *a)
{
	if(a == NULL || a->digits == NULL)return;
	a->size++;
	if(a->capacity < a->size)
	{
		unsigned int new_cap = a->capacity * 2;
		unsigned long long *tmp = a->digits;
		a->digits = (unsigned long long *)malloc(sizeof(unsigned long long) * new_cap);
		if(a->digits == NULL || a->size > new_cap)fprintf(stderr, "biguint extending failed.\n");
		for(unsigned int i = 0; i < a->capacity; i++)a->digits[i] = tmp[i];
		a->capacity = new_cap;
		free(p);
	}
	a->digits[a->size - 1] = 0;
}

static void _extend(biguint *a, unsigned int amount)
{
	if(a == NULL || a->digits == NULL)return;
	unsigned long long pre_size = a->size;
	a->size += amount;
	if(a->capacity < a->size)
	{
		unsigned int new_cap = max2pow(a->size);
		unsigned long long *tmp = a->digits;
		a->digits = (unsigned long long *)malloc(sizeof(unsigned long long) * new_cap);
		if(a->digits == NULL || a->size > new_cap)fprintf(stderr, "biguint extending failed.\n");
		for(unsigned int i = 0; i < pre_size; i++)a->digits[i] = tmp[i];
		a->capacity = new_cap;
		free(p);
	}
	for(unsigned int i = pre_size; i < pre_size + amount; i++)a->digits[i] = 0;
}

void fix_carry(biguint *num)
{
	
}
//a += b
void big_add(biguint *a, const biguint *b)
{
	
}
//べき剰余計算(バイナリ法)(引数base:a, exp:b, mod:nとして、a^b mod nを計算)
int modpow(int base, unsigned int exp, int mod)
{
    int result = 1;//結果が格納される変数
    unsigned int mask = 0x80000000;//bの1ビットのみを取り出すためのマスク用変数
    while(mask)//第0ビットとマスクするまで繰り返し
    {
        //二乗して剰余をとる
        result *= result;
        result %= mod;

        //対象ビットが1だったらaをかけて剰余をとる
        if(exp & mask)
        {
            result *= base;
            result %= mod;
        } 
        //次ビットに遷移
        mask >>= 1;
    }
    return result;
}
