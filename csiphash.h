#include <stddef.h>
struct siphash {
    uint64_t b;
    uint64_t v0;
    uint64_t v1;
    uint64_t v2;
    uint64_t v3;
};
typedef struct siphash siphash;
void siphashinit (siphash *sh, size_t src_sz, const uint64_t key[2]);
void siphashadd64bits (siphash *sh, const void *in);
uint64_t siphashfinish (siphash *sh, const void *src, size_t src_sz);
uint64_t siphash24(const void *src, size_t src_sz, const uint64_t key[2]);
