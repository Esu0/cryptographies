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

static const unsigned int md5_K[64] = {
	0xD76AA478ul, 0xE8C7B756ul, 0x242070DBul, 0xC1BDCEEEul,
	0xF57C0FAFul, 0x4787C62Aul, 0xA8304613ul, 0xFD469501ul,
	0x698098D8ul, 0x8B44F7AFul, 0xFFFF5BB1ul, 0x895CD7BEul,
	0x6B901122ul, 0xFD987193ul, 0xA679438Eul, 0x49B40821ul,
	0xF61E2562ul, 0xC040B340ul, 0x265E5A51ul, 0xE9B6C7AAul,
	0xD62F105Dul, 0x02441453ul, 0xD8A1E681ul, 0xE7D3FBC8ul,
	0x21E1CDE6ul, 0xC33707D6ul, 0xF4D50D87ul, 0x455A14EDul,
	0xA9E3E905ul, 0xFCEFA3F8ul, 0x676F02D9ul, 0x8D2A4C8Aul,
	0xFFFA3942ul, 0x8771F681ul, 0x6D9D6122ul, 0xFDE5380Cul,
	0xA4BEEA44ul, 0x4BDECFA9ul, 0xF6BB4B60ul, 0xBEBFBC70ul,
	0x289B7EC6ul, 0xEAA127FAul, 0xD4EF3085ul, 0x04881D05ul,
	0xD9D4D039ul, 0xE6DB99E5ul, 0x1FA27CF8ul, 0xC4AC5665ul,
	0xF4292244ul, 0x432AFF97ul, 0xAB9423A7ul, 0xFC93A039ul,
	0x655B59C3ul, 0x8F0CCC92ul, 0xFFEFF47Dul, 0x85845DD1ul,
	0x6FA87E4Ful, 0xFE2CE6E0ul, 0xA3014314ul, 0x4E0811A1ul,
	0xF7537E82ul, 0xBD3AF235ul, 0x2AD7D2BBul, 0xEB86D391ul
};

inline static unsigned int md5_F1(unsigned int x, unsigned int y, unsigned int z)
{
	return (x & y) | (~x & z);
}

inline static unsigned int md5_F2(unsigned int x, unsigned int y, unsigned int z)
{
	return (x & z) | (y & ~z);
}

inline static unsigned int md5_F3(unsigned int x, unsigned int y, unsigned int z)
{
	return x ^ y ^ z;
}

inline static unsigned int md5_F4(unsigned int x, unsigned int y, unsigned int z)
{
	return y ^ (x | ~z);
}

static unsigned int md5_lrotate(unsigned int x, unsigned char s)
{
	return (x << s) | (x >> (32 - s));
}

static void md5_update_buffer(
	unsigned int *pa,
	unsigned int *pb,
	unsigned int *pc,
	unsigned int *pd,
	const unsigned int *p
)
{
	unsigned int A = *pa, B = *pb, C = *pc, D = *pd;
	A = B + md5_lrotate(A + md5_F1(B, C, D) + p[0] + md5_K[0], 7);
	D = A + md5_lrotate(D + md5_F1(A, B, C) + p[1] + md5_K[1], 12);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[2] + md5_K[2], 17);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[3] + md5_K[3], 22);
	A = B + md5_lrotate(A + md5_F1(B, C, D) + p[4] + md5_K[4], 7);
	D = A + md5_lrotate(D + md5_F1(A, B, C) + p[5] + md5_K[5], 12);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[6] + md5_K[6], 17);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[7] + md5_K[7], 22);
	A = B + md5_lrotate(A + md5_F1(B, C, D) + p[8] + md5_K[8], 7);
	D = A + md5_lrotate(D + md5_F1(A, B, C) + p[9] + md5_K[9], 12);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[10] + md5_K[10], 17);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[11] + md5_K[11], 22);
	A = B + md5_lrotate(A + md5_F1(B, C, D) + p[12] + md5_K[12], 7);
	D = A + md5_lrotate(D + md5_F1(A, B, C) + p[13] + md5_K[13], 12);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[14] + md5_K[14], 17);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[15] + md5_K[15], 22);

	A = B + md5_lrotate(A + md5_F2(B, C, D) + p[1] + md5_K[16], 5);
	D = A + md5_lrotate(D + md5_F2(A, B, C) + p[6] + md5_K[17], 9);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[11] + md5_K[18], 14);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[0] + md5_K[19], 20);
	A = B + md5_lrotate(A + md5_F2(B, C, D) + p[5] + md5_K[20], 5);
	D = A + md5_lrotate(D + md5_F2(A, B, C) + p[10] + md5_K[21], 9);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[15] + md5_K[22], 14);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[4] + md5_K[23], 20);
	A = B + md5_lrotate(A + md5_F2(B, C, D) + p[9] + md5_K[24], 5);
	D = A + md5_lrotate(D + md5_F2(A, B, C) + p[14] + md5_K[25], 9);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[3] + md5_K[26], 14);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[8] + md5_K[27], 20);
	A = B + md5_lrotate(A + md5_F2(B, C, D) + p[13] + md5_K[28], 5);
	D = A + md5_lrotate(D + md5_F2(A, B, C) + p[2] + md5_K[29], 9);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[7] + md5_K[30], 14);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[12] + md5_K[31], 20);

	A = B + md5_lrotate(A + md5_F3(B, C, D) + p[5] + md5_K[32], 4);
	D = A + md5_lrotate(D + md5_F3(A, B, C) + p[8] + md5_K[33], 11);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[11] + md5_K[34], 16);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[14] + md5_K[35], 23);
	A = B + md5_lrotate(A + md5_F3(B, C, D) + p[1] + md5_K[36], 4);
	D = A + md5_lrotate(D + md5_F3(A, B, C) + p[4] + md5_K[37], 11);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[7] + md5_K[38], 16);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[10] + md5_K[39], 23);
	A = B + md5_lrotate(A + md5_F3(B, C, D) + p[13] + md5_K[40], 4);
	D = A + md5_lrotate(D + md5_F3(A, B, C) + p[0] + md5_K[41], 11);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[3] + md5_K[42], 16);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[6] + md5_K[43], 23);
	A = B + md5_lrotate(A + md5_F3(B, C, D) + p[9] + md5_K[44], 4);
	D = A + md5_lrotate(D + md5_F3(A, B, C) + p[12] + md5_K[45], 11);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[15] + md5_K[46], 16);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[2] + md5_K[47], 23);

	A = B + md5_lrotate(A + md5_F4(B, C, D) + p[0] + md5_K[48], 6);
	D = A + md5_lrotate(D + md5_F4(A, B, C) + p[7] + md5_K[49], 10);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[14] + md5_K[50], 15);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[5] + md5_K[51], 21);
	A = B + md5_lrotate(A + md5_F4(B, C, D) + p[12] + md5_K[52], 6);
	D = A + md5_lrotate(D + md5_F4(A, B, C) + p[3] + md5_K[53], 10);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[10] + md5_K[54], 15);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[1] + md5_K[55], 21);
	A = B + md5_lrotate(A + md5_F4(B, C, D) + p[8] + md5_K[56], 6);
	D = A + md5_lrotate(D + md5_F4(A, B, C) + p[15] + md5_K[57], 10);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[6] + md5_K[58], 15);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[13] + md5_K[59], 21);
	A = B + md5_lrotate(A + md5_F4(B, C, D) + p[4] + md5_K[60], 6);
	D = A + md5_lrotate(D + md5_F4(A, B, C) + p[11] + md5_K[61], 10);
	C = D + md5_lrotate(C + md5_F1(D, A, B) + p[2] + md5_K[62], 15);
	B = C + md5_lrotate(B + md5_F1(C, D, A) + p[9] + md5_K[63], 21);
	*pa += A; *pb += B; *pc += C; *pd += D;
}

unsigned int md5_byte_reverse(unsigned int x)
{
	return (x << 24) | ((x << 8) & 0x00ff0000) | ((x >> 8) & 0x0000ff00) | (x >> 24);
}

void dump(size_t byte_num, const void *data)
{
	const byte *p = (const byte *)data;
	for(size_t i = 0; i < byte_num; i++)printf("%2x", p[i]);
	printf("\n");
}

bit128 md5(size_t bit_num, const void *data)
{
	const unsigned int *mainptr = (const unsigned int *)data;
	size_t size = bit_num >> 3;
	unsigned short remainder = bit_num & 0x00000000000001ffull;
	byte endsize = remainder < 448 ? 64 : 128;
	byte *added = (byte *)malloc(endsize);
	if(added == NULL)exit(1);
	size_t index;

	unsigned int A = 0x67452301;
	unsigned int B = 0xefcdab89;
	unsigned int C = 0x98badcfe;
	unsigned int D = 0x10325476;

	const byte *pend = (byte *)data + ((bit_num - remainder) >> 3);
	byte remainder_r = remainder & (unsigned short)0x07;
	bit128 result;
	for(index = 0; index < (remainder >> 3); index++)
	{
		added[index] = pend[index];
	}
	added[index] = (pend[index] & ~((byte)0xff >> remainder_r)) | ((byte)0x80 >> remainder_r);
	for(index++; index < endsize; index++)added[index] = 0;
	index -= 8;
	added[index] = bit_num & 0x00000000000000ffull;
	index++;
	added[index] = (bit_num >> 8) & 0x00000000000000ffull;
	index++;
	added[index] = (bit_num >> 16) & 0x00000000000000ffull;
	index++;
	added[index] = (bit_num >> 24) & 0x00000000000000ffull;
	index++;
	added[index] = (bit_num >> 32) & 0x00000000000000ffull;
	index++;
	added[index] = (bit_num >> 40) & 0x00000000000000ffull;
	index++;
	added[index] = (bit_num >> 48) & 0x00000000000000ffull;
	index++;
	added[index] = (bit_num >> 56) & 0x00000000000000ffull;

	//dump(endsize, added);
	for(index = 0; index + 16 < size; index += 16)
	{
		const unsigned int *p = mainptr + index;
		md5_update_buffer(&A, &B, &C, &D, p);
	}
	md5_update_buffer(&A, &B, &C, &D, (unsigned int *)added);
	if(endsize & (byte)0x80)md5_update_buffer(&A, &B, &C, &D, (unsigned int *)added + 16);
	free(added);
	result._dat[0] = md5_byte_reverse(A);
	result._dat[1] = md5_byte_reverse(B);
	result._dat[2] = md5_byte_reverse(C);
	result._dat[3] = md5_byte_reverse(D);
	return result;
}

void print128(bit128 x)
{
	printf("%08x%08x%08x%08x\n", x._dat[0], x._dat[1], x._dat[2], x._dat[3]);
}

typedef union __bit1600
{
	unsigned long long words[5][5];
	byte bytes[200];
} bit1600;

static const byte rotoffset_table[5][5] = {
	{ 0,  1, 62, 28, 27},
	{36, 44,  6, 55, 20},
	{ 3, 10, 43, 25, 39},
	{41, 45, 15, 21,  8},
	{18,  2, 61, 56, 14}
};

static const word iota_constant[24] = {
	0x0000000000000001ull, 0x0000000000008082ull, 0x800000000000808aull,
	0x8000000080008000ull, 0x000000000000808bull, 0x0000000080000001ull,
	0x8000000080008081ull, 0x8000000000008009ull, 0x000000000000008aull,
	0x0000000000000088ull, 0x0000000080008009ull, 0x000000008000000aull,
	0x000000008000808bull, 0x800000000000008bull, 0x8000000000008089ull,
	0x8000000000008003ull, 0x8000000000008002ull, 0x8000000000000080ull,
	0x000000000000800aull, 0x800000008000000aull, 0x8000000080008081ull,
	0x8000000000008080ull, 0x0000000080000001ull, 0x8000000080008008ull,
};

word lrotatew(word w, byte amount)
{
	return (w << amount) | (w >> (64 - amount));
}

static void keccakF1600_round_theta(bit1600 *state)
{
	word c[5];
	word d[5];
	c[0] = state->words[0][0] ^ state->words[1][0] ^ state->words[2][0] ^ state->words[3][0] ^ state->words[4][0];
	c[1] = state->words[0][1] ^ state->words[1][1] ^ state->words[2][1] ^ state->words[3][1] ^ state->words[4][1];
	c[2] = state->words[0][2] ^ state->words[1][2] ^ state->words[2][2] ^ state->words[3][2] ^ state->words[4][2];
	c[3] = state->words[0][3] ^ state->words[1][3] ^ state->words[2][3] ^ state->words[3][3] ^ state->words[4][3];
	c[4] = state->words[0][4] ^ state->words[1][4] ^ state->words[2][4] ^ state->words[3][4] ^ state->words[4][4];
	d[0] = c[4] ^ ((c[1] << 1) | (c[1] >> 63));
	d[1] = c[0] ^ ((c[2] << 1) | (c[2] >> 63));
	d[2] = c[1] ^ ((c[3] << 1) | (c[3] >> 63));
	d[3] = c[2] ^ ((c[4] << 1) | (c[4] >> 63));
	d[4] = c[3] ^ ((c[0] << 1) | (c[0] >> 63));

	for(byte i = 0; i < 5; i++)
		for(byte j = 0; j < 5; j++)
			state->words[i][j] ^= d[j];
}

static void keccakF1600_round_rho(bit1600 *state)
{
	for(byte i = 0; i < 5; i++)
		for(byte j = 0; j < 5; j++)
			state->words[i][j] = lrotatew(state->words[i][j], rotoffset_table[i][j]);
}

static void keccakF1600_round_pi(bit1600 *state)
{
	word tmp;
	tmp = state->words[0][1];
	state->words[0][1] = state->words[0][3];
	state->words[0][3] = state->words[0][4];
	state->words[0][4] = state->words[0][2];
	state->words[0][2] = tmp;

	tmp = state->words[1][0];
	state->words[1][0] = state->words[1][1];
	state->words[1][1] = state->words[1][3];
	state->words[1][3] = state->words[1][4];
	state->words[1][4] = tmp;

	tmp = state->words[2][0];
	state->words[2][0] = state->words[2][2];
	state->words[2][2] = state->words[2][3];
	state->words[2][3] = state->words[2][1];
	state->words[2][1] = tmp;

	tmp = state->words[3][0];
	state->words[3][0] = state->words[3][3];
	state->words[3][3] = state->words[3][2];
	state->words[3][2] = state->words[3][4];
	state->words[3][4] = tmp;

	tmp = state->words[4][0];
	state->words[4][0] = state->words[4][4];
	state->words[4][4] = state->words[4][1];
	state->words[4][1] = state->words[4][2];
	state->words[4][2] = tmp;
}

static void keccakF1600_round_chi(bit1600 *state)
{
	word cpy[5];
	for(byte i = 0; i < 5; i++)
	{
		cpy[0] = ~state->words[i][1] & state->words[i][2];
		cpy[1] = ~state->words[i][2] & state->words[i][3];
		cpy[2] = ~state->words[i][3] & state->words[i][4];
		cpy[3] = ~state->words[i][4] & state->words[i][0];
		cpy[4] = ~state->words[i][0] & state->words[i][1];
		state->words[i][0] ^= cpy[0];
		state->words[i][1] ^= cpy[1];
		state->words[i][2] ^= cpy[2];
		state->words[i][3] ^= cpy[3];
		state->words[i][4] ^= cpy[4];
	}
}

static void keccakF1600_round_iota(bit1600 *state, unsigned char iter)
{
	state->words[0][0] ^= iota_constant[iter];
}

static void keccakF1600_round(bit1600 *state, unsigned char i)
{
	keccakF1600_round_theta(state);
	keccakF1600_round_rho(state);
	keccakF1600_round_pi(state);
	keccakF1600_round_chi(state);
	keccakF1600_round_iota(state, i);
}

static void keccakF1600(bit1600 *state)
{
	for(unsigned char i = 0; i < 24; i++)
	{
		keccakF1600_round(state, i);
	}
}

static void dump1600(bit1600 *a)
{
	printf("words\n");
	for(byte i = 0; i < 5; i++)
		for(byte j = 0; j < 5; j++)
			printf("%016llx", a->words[i][j]);
	printf("\nbytes\n");
	for(byte i = 0; i < 200; i++)
		printf("%02x", a->bytes[i]);
	printf("\n");
}

bit256 keccak256(size_t bit_num, const void *dat)
{
	const byte *dat_byte = (const byte *)dat;
	bit1600 state;
	size_t byte_num = bit_num >> 3;
	for(byte i = 0; i < 5; i++)
		for(byte j = 0; j < 5; j++)
			state.words[i][j] = 0;
	size_t byte_now;
	const byte *p = dat_byte;
	for(byte_now = 0; byte_now + 136 < byte_num; byte_now += 136)
	{
		p = dat_byte + byte_now;
		for(size_t i = 0; i < 136; i++)
		{
			state.bytes[i] ^= p[i];
		}
		keccakF1600(&state);
	}

	/* パディング処理 */
	p = dat_byte + byte_now;
	size_t remain_bit = bit_num - (byte_now << 3);
	size_t remain_byte = remain_bit >> 3;
	byte remain_bit_remain = remain_bit & 0x0000000000000007ull;
	size_t index;
	for(index = 0; index < remain_byte; index++)
		state.bytes[index] ^= p[index];
	if(remain_bit_remain)
	{
		state.bytes[index] ^= ((p[index] & ((byte)0xff >> (8 - remain_bit_remain))) | ((byte)0x06 << remain_bit_remain));
		if(index < 135)
			state.bytes[index + 1] ^= (byte)0x06 >> (8 - remain_bit_remain);
	}
	else
	{
		state.bytes[index] ^= (byte)0x06;
	}
	state.bytes[135] ^= (byte)0x80;

	keccakF1600(&state);

	bit256 result;
	for(byte i = 0; i < 4; i++)
	{
		result._dat[i] = state.words[0][i];
	}
	return result;
}

void print256(bit256 x)
{
	printf("%016llx%016llx%016llx%016llx\n", x._dat[3], x._dat[2], x._dat[1], x._dat[0]);
}
