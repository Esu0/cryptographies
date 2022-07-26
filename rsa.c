//#include"crypts.h"
#include<stdio.h>
#include<stdlib.h>
#define _BIGINT_CARRY_MASK 0x00000000ffffffffull
#define _BIGINT_DIGIT_MAX 0x0fffffffffffffffull
const int p = 103, q = 53, n = 5459, e = 433, d = 49;

//4294967295進数の多倍長
typedef struct __biguint
{
	unsigned long long *digits;
	unsigned int size;
	unsigned int capacity;
	unsigned long long max_value;
} biguint;

static void _delete_zero(biguint *);

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

/* sizeの大きさの配列で多倍長を生成、中身は0で初期化 */
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

/* 使い終わったら実行
	メモリ解放
*/
void biguint_delete(biguint *integer)
{
	if(integer->digits != NULL)
	{
		free(integer->digits);
	}
}

/* 標準の符号なし整数型からbiguintへの変換 */
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
	p[0] = (unsigned int)(_BIGINT_CARRY_MASK & a);
	p[1] = (unsigned int)(a >> 32);
	new_inst.digits = p;
	new_inst.size = 2;
	new_inst.capacity = 2;
	new_inst.max_value = UINT_MAX;
	_delete_zero(&new_inst);
	return new_inst;
}

/* 1ワードだけ延長、他は_extendと同じ */
static void _extend1digit(biguint *a)
{
	if(a == NULL || a->digits == NULL)return;
	a->size++;
	if(a->capacity < a->size)
	{
		unsigned int new_cap = a->capacity * 2;
		unsigned long long *tmp = a->digits;
		a->digits = (unsigned long long *)malloc(sizeof(unsigned long long) * new_cap);
		if(a->digits == NULL || a->size > new_cap)
		{
			fprintf(stderr, "biguint extending failed.\n");
			return;
		}
		unsigned int tmp0 = a->capacity;
		for(unsigned int i = 0; i < tmp0; i++)a->digits[i] = tmp[i];
		a->capacity = new_cap;
		free(tmp);
	}
	a->digits[a->size - 1] = 0;
}

/* sizeをamountだけ延長
	capacity,sizeは自動で更新
	max_valueは手を加えない
*/
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
		if(a->digits == NULL || a->size > new_cap)
		{
			fprintf(stderr, "biguint extending failed.\n");
			return;
		}
		for(unsigned int i = 0; i < pre_size; i++)a->digits[i] = tmp[i];
		a->capacity = new_cap;
		free(tmp);
	}
	for(unsigned int i = pre_size; i < pre_size + amount; i++)a->digits[i] = 0;
}

/* sizeを変更する(縮小にも対応)
	縮小する場合はcapacityは変化しない
*/
static void _resize(biguint *a, unsigned int new_size)
{
	if(a == NULL || a->digits == NULL)return;
	if(a->size > new_size)a->size = new_size;
	else if(a->size < new_size)_extend(a, new_size - a->size);
}

/* 先頭の0を切り詰める */
static void _delete_zero(biguint *a)
{
	while(a->size > 1 && a->digits[a->size - 1] == 0)a->size--;
}

/* 無効な数かを判断 */
int is_null(const biguint *num)
{
	return num == NULL || num->digits == NULL;
}

/* 溜まったキャリーをまとめてリセット */
void fix_carry(biguint *num)
{
	if(num == NULL || num->digits == NULL)return;
	unsigned long long *digits = num->digits;
	unsigned int carry = 0;
	unsigned int i;
	for(i = 0; i < num->size; i++)
	{
		digits[i] += carry;
		carry = digits[i] >> 32;
		digits[i] &= _BIGINT_CARRY_MASK;
	}
	if(carry != 0)
	{
		_extend1digit(num);
		num->digits[num->size - 1] = carry;
	}
	num->max_value = UINT_MAX;
	_delete_zero(num);
}

/* capacityを減らす */
void shrink(biguint *num)
{
	if(num == NULL || num->digits == NULL)return;
	unsigned int new_cap = max2pow(num->size);
	if(new_cap <= num->capacity)return;
	unsigned long long *tmp = num->digits;
	num->digits = (unsigned long long *)malloc(sizeof(unsigned long long) * new_cap);
	if(num->digits == NULL)
	{
		fprintf(stderr, "biguint shrink failed.");
		return;
	}
	for(unsigned int i = 0; i < num->size; i++)
	{
		num->digits[i] = tmp[i];
	}
	free(tmp);
	num->capacity = new_cap;
}

/*キャリーを処理しながら足し算をする*/
static void big_add_with_carry_fix(biguint *a, const biguint *b)
{
	unsigned long long carry = 0;
	unsigned int i;
	unsigned long long *pa = a->digits;
	const unsigned long long *pb = b->digits;
	for(i = 0; i < a->size; i++)
	{
		if(i < b->size)
		{
			carry = (pa[i] >> 32) + (pb[i] >> 32) + (carry >> 32);
			pa[i] = (pa[i] & _BIGINT_CARRY_MASK) + (pb[i] & _BIGINT_CARRY_MASK);
			carry += pa[i] >> 32;
			pa[i] &= _BIGINT_CARRY_MASK;
		}
		else break;
	}
	if(a->size < b->size)
	{
		_extend(a, b->size - a->size);
		pa = a->digits;
		for(; i < b->size; i++)
		{
			carry = (pb[i] >> 32) + (carry >> 32);
			pa[i] = pb[i] & _BIGINT_CARRY_MASK;
			carry += pa[i] >> 32;
			pa[i] &= _BIGINT_CARRY_MASK;
		}
	}
	if(carry)
	{
		if(i >= a->size)
		{
			_extend1digit(a);
			pa = a->digits;
		}
		pa[i] = carry & _BIGINT_CARRY_MASK;
		carry >>= 32;
		i++;
		if(carry)
		{
			if(i >= a->size)
			{
				_extend1digit(a);
				pa = a->digits;
			}
			pa[i] = carry;
		}
	}

	_delete_zero(a);
	a->max_value = ULONG_MAX;
}

/* a += b */
void big_add(biguint *a, const biguint *b)
{
	if(is_null(a) || is_null(b))return;
	if(_BIGINT_DIGIT_MAX - a->max_value < b->max_value)
	{
		big_add_with_carry_fix(a, b);
	}
	else
	{
		unsigned int i;
		a->max_value += b->max_value;
		for(i = 0; i < a->size; i++)
		{
			if(i < b->size)
			{
				a->digits[i] += b->digits[i];
			}
			else break;
		}
		if(a->size < b->size)
		{
			_extend(a, b->size - a->size);
			for(; i < b->size; i++)
			{
				a->digits[i] = b->digits[i];
			}
		}
	}
}

/* a -= b */
void big_sub(biguint *a, const biguint *b)
{
	if(is_null(a) || is_null(b))return;
	if(a->size < b->size)
	{
		a->size = 1;
		a->digits[0] = 0;
		return;
	}
	unsigned int i;
	long long carry = 0;
	for(i = 0; i < b->size; i++)
	{
		unsigned long long sub;
		if(carry < 0)
		{
			sub = b->digits[i] + (unsigned long long)(-carry);
		}
		else
		{
			sub = b->digits[i];
			a->digits[i] += (unsigned long long)carry;
		}
		carry = (long long)(a->digits[i] >> 32) - (long long)(sub >> 32);
		sub &= _BIGINT_CARRY_MASK;
		a->digits[i] &= _BIGINT_CARRY_MASK;
		if(a->digits[i] < sub)
		{
			a->digits[i] += 0x0000000100000000 - sub;
			carry--;
		}
		else
		{
			a->digits[i] -= sub;
		}
	}

	for(; i < a->size; i++)
	{
		unsigned long long sub;
		if(carry < 0)
		{
			sub = (unsigned long long)(-carry);
		}
		else
		{
			sub = 0;
			a->digits[i] += (unsigned long long)carry;
		}
		//途中から
	}
}

/* a *= b */
void big_mul(biguint *a, const biguint *b)
{

}

/* a %= b */
void big_rem(biguint *a, const biguint *b)
{

}

/* 多倍長整数出力 */
void biguint_print(biguint *num)
{
	if(is_null(num))return;
	fix_carry(num);
	for(unsigned int i = num->size; i > 0;)
	{
		i--;
		printf("%08lx", (long unsigned int)(num->digits[i]));
	}
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

int main()
{
	biguint a = biguint_with_integer(0x333344445555), b = biguint_with_integer(0x00000000);
	biguint_print(&a);
	printf("\n");
	big_add(&a, &b);
	biguint_print(&a);
	printf("\n");
	biguint_delete(&a);
	biguint_delete(&b);
}