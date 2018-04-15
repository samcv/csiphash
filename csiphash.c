/* <MIT License>
 Copyright (c) 2013  Marek Majkowski <marek@popcount.org>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 </MIT License>

 Original location:
	https://github.com/majek/csiphash/

 Solution inspired by code from:
	Samuel Neves (supercop/crypto_auth/siphash24/little)
	djb (supercop/crypto_auth/siphash24/little2)
	Jean-Philippe Aumasson (https://131002.net/siphash/siphash24.c)


*/

#include <stdint.h>
#include "csiphash.h"

#define ROTATE(x, b) (uint64_t)( ((x) << (b)) | ( (x) >> (64 - (b))) )

#define HALF_ROUND(a,b,c,d,s,t) \
	a += b; c += d;             \
	b = ROTATE(b, s) ^ a;       \
	d = ROTATE(d, t) ^ c;       \
	a = ROTATE(a, 32);

#define DOUBLE_ROUND(v0,v1,v2,v3)  \
	HALF_ROUND(v0,v1,v2,v3,13,16); \
	HALF_ROUND(v2,v1,v0,v3,17,21); \
	HALF_ROUND(v0,v1,v2,v3,13,16); \
	HALF_ROUND(v2,v1,v0,v3,17,21);

void siphashinit (siphash *sh, size_t src_sz, const uint64_t key[2]) {
	uint64_t k0 = MVM_TO_LITTLE_ENDIAN_64(key[0]);
	uint64_t k1 = MVM_TO_LITTLE_ENDIAN_64(key[1]);
	sh->b = (uint64_t)src_sz << 56;
	sh->v0 = k0 ^ 0x736f6d6570736575ULL;
	sh->v1 = k1 ^ 0x646f72616e646f6dULL;
	sh->v2 = k0 ^ 0x6c7967656e657261ULL;
	sh->v3 = k1 ^ 0x7465646279746573ULL;
}
void siphashadd64bits (siphash *sh, const void *in) {
    uint64_t mi = MVM_TO_LITTLE_ENDIAN_64(*(uint64_t*)in);
    sh->v3 ^= mi;
    DOUBLE_ROUND(sh->v0,sh->v1,sh->v2,sh->v3);
    sh->v0 ^= mi;
}
uint64_t siphashfinish (siphash *sh, const void *src, size_t src_sz) {
	const uint64_t *in = (uint64_t*)src;
	uint64_t t  = 0;
	uint8_t *pt = (uint8_t *)&t;
	uint8_t *m  = (uint8_t *)in;
	switch (src_sz) {
		/* Falls through */
		case 7: pt[6] = m[6];
		/* Falls through */
		case 6: pt[5] = m[5];
		/* Falls through */
		case 5: pt[4] = m[4];
		/* Falls through */
		case 4:
			*((uint32_t*)&pt[0]) = *((uint32_t*)&m[0]);
			break;
		case 3: pt[2] = m[2];
		/* Falls through */
		case 2: pt[1] = m[1];
		/* Falls through */
		case 1: pt[0] = m[0];
	}
	sh->b |= MVM_TO_LITTLE_ENDIAN_64(t);
	sh->v3 ^= sh->b;
	DOUBLE_ROUND(sh->v0,sh->v1,sh->v2,sh->v3);
	sh->v0 ^= sh->b;
	sh->v2 ^= 0xff;
	DOUBLE_ROUND(sh->v0,sh->v1,sh->v2,sh->v3);
	DOUBLE_ROUND(sh->v0,sh->v1,sh->v2,sh->v3);
	return (sh->v0 ^ sh->v1) ^ (sh->v2 ^ sh->v3);
}
uint64_t siphash24(const void *src, size_t src_sz, const uint64_t key[2]) {
	siphash sh;
	siphashinit(&sh, src_sz, key);
	const uint64_t *in = (uint64_t*)src;
	while (src_sz >= 8) {
		siphashadd64bits(&sh, (uint8_t*)in);
		in += 1; src_sz -= 8;
	}
	return siphashfinish(&sh, (uint8_t *)in, src_sz);
}
