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
#if defined(MVM_HASH_FORCE_LITTLE_ENDIAN)
    #if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && \
        __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #  define _le64toh(x) ((uint64_t)(x))
    #elif defined(_WIN32)
    /* Windows is always little endian, unless you're on xbox360
       http://msdn.microsoft.com/en-us/library/b0084kay(v=vs.80).aspx */
    #  define _le64toh(x) ((uint64_t)(x))
    #elif defined(__APPLE__)
    #  include <libkern/OSByteOrder.h>
    #  define _le64toh(x) OSSwapLittleToHostInt64(x)
    #else

    /* See: http://sourceforge.net/p/predef/wiki/Endianness/ */
    #  if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    #    include <sys/endian.h>
    #  else
    #    include <endian.h>
    #  endif
    #  if defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && \
        __BYTE_ORDER == __LITTLE_ENDIAN
    #    define _le64toh(x) ((uint64_t)(x))
    #  else
    #    define _le64toh(x) le64toh(x)
    #  endif

    #endif
#else
    #define _le64toh(x) ((uint64_t)(x))
#endif
