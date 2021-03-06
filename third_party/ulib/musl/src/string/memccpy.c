#include <limits.h>
#include <stdint.h>
#include <string.h>

#define ALIGN (sizeof(size_t) - 1)
#define ONES ((size_t)-1 / UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) ((x)-ONES & ~(x)&HIGHS)

void* memccpy(void* restrict dest, const void* restrict src, int c, size_t n) {
    unsigned char* d = dest;
    const unsigned char* s = src;
    size_t *wd, k;
    const size_t* ws;

    c = (unsigned char)c;
    if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN)) {
        for (; ((uintptr_t)s & ALIGN) && n && (*d = *s) != c; n--, s++, d++)
            ;
        if ((uintptr_t)s & ALIGN)
            goto tail;
        k = ONES * c;
        wd = (void*)d;
        ws = (const void*)s;
        for (; n >= sizeof(size_t) && !HASZERO(*ws ^ k); n -= sizeof(size_t), ws++, wd++)
            *wd = *ws;
        d = (void*)wd;
        s = (const void*)ws;
    }
    for (; n && (*d = *s) != c; n--, s++, d++)
        ;
tail:
    if (*s == c)
        return d + 1;
    return 0;
}
