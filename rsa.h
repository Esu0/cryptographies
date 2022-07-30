#pragma once

#ifndef _RSA_H_
#define _RSA_H_

//4294967295進数の多倍長
typedef struct __biguint
{
	unsigned long long *digits;
	unsigned int size;
	unsigned int capacity;
	unsigned long long max_value;
} biguint;

biguint biguint_new(unsigned int);
void biguint_delete(biguint *);
biguint biguint_with_integer(unsigned long long);
void shrink(biguint *);
void big_add(biguint *, const biguint *);
void big_sub(biguint *, const biguint *);
void big_mul_normal(unsigned long long *, const unsigned long long *, const unsigned long long *, unsigned int);
void _karatsuba_unsafe(unsigned long long *, const unsigned long long *, const unsigned long long *, unsigned int);
void biguint_print(biguint *);

#endif