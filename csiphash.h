#include <stddef.h>
struct siphash {
    const uint64_t *_key;
    uint64_t k0;
    uint64_t k1;
    uint64_t b;
    uint64_t v0;
    uint64_t v1;
    uint64_t v2;
    uint64_t v3;
};
typedef struct siphash siphash;
void siphashinit (unsigned long src_sz, const char key[16], siphash *sh);
void siphashadd64bits (siphash *sh, const uint64_t *in);
uint64_t siphashfinish (siphash *sh, const void *src, size_t src_sz);
uint64_t siphash24(const void *src, unsigned long src_sz, const char key[16]);
