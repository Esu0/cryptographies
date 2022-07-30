#pragma once

#ifndef _CRYPTS_H_
#define _CRYPTS_H_

#include<stdio.h>
#include<stdlib.h>

typedef struct _bit128
{
	unsigned int _dat[4];
} bit128;

typedef struct _bit256
{
	unsigned long long _dat[4];
} bit256;

typedef unsigned char byte;
typedef unsigned long long word;

unsigned int md5_byte_reverse(unsigned int x);

bit128 md5(size_t, const void *);

void print128(bit128);

typedef union __bit1600
{
	unsigned long long words[5][5];
	byte bytes[200];
} bit1600;

word lrotatew(word, byte);

bit256 keccak256(size_t, const void *);
void print256(bit256);

#endif