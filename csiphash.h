#include <stddef.h>
struct siphash {
    uint64_t b;
    uint64_t v0;
    uint64_t v1;
    uint64_t v2;
    uint64_t v3;
};
typedef struct siphash siphash;
void siphashinit (siphash *sh, size_t src_sz, const char key[16]);
void siphashadd64bits (siphash *sh, const void *src);
uint64_t siphashfinish (siphash *sh, const void *src, size_t src_sz);
uint64_t siphash24(const void *src, size_t src_sz, const char key[16]);
#if defined(MVM_HASH_FORCE_LITTLE_ENDIAN)
    #if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && \
        __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #  define MVM_TO_LITTLE_ENDIAN_64(x) ((uint64_t)(x))
    #  define MVM_TO_LITTLE_ENDIAN_32(x) ((uint32_t)(x))
    #elif defined(_WIN32)
    /* Windows is always little endian, unless you're on xbox360
       http://msdn.microsoft.com/en-us/library/b0084kay(v=vs.80).aspx */
    #  define MVM_TO_LITTLE_ENDIAN_64(x) ((uint64_t)(x))
    #  define MVM_TO_LITTLE_ENDIAN_32(x) ((uint32_t)(x))
    #elif defined(__APPLE__)
    #  include <libkern/OSByteOrder.h>
    #  define MVM_TO_LITTLE_ENDIAN_64(x) OSSwapLittleToHostInt64(x)
    #  define MVM_TO_LITTLE_ENDIAN_32(x) OSSwapLittleToHostInt32(x)
    #else

    /* See: http://sourceforge.net/p/predef/wiki/Endianness/ */
    #  if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    #    include <sys/endian.h>
    #  else
    #    include <endian.h>
    #  endif
    #  if defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && \
        __BYTE_ORDER == __LITTLE_ENDIAN
    #    define MVM_TO_LITTLE_ENDIAN_64(x) ((uint64_t)(x))
    #    define MVM_TO_LITTLE_ENDIAN_32(x) ((uint32_t)(x))
    #  else
    #    define MVM_TO_LITTLE_ENDIAN_64(x) le64toh(x)
    #    define MVM_TO_LITTLE_ENDIAN_32(x) le32toh(x)
    #  endif

    #endif
#else
    #define MVM_TO_LITTLE_ENDIAN_64(x) ((uint64_t)(x))
    #define MVM_TO_LITTLE_ENDIAN_32(x) ((uint32_t)(x))
#endif
