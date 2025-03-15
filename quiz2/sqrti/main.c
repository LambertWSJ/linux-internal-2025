#include <assert.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "list.h"

#define N 100000
#define clz32(x) clz2(x, 0)

static const int mask[] = {0, 8, 12, 14};
static const int magic[] = {2, 1, 0, 0};

unsigned clz2(uint32_t x, int c)
{
    if (!x && !c)
        return 32;

    uint32_t upper = (x >> (16 >> c));
    uint32_t lower = (x & (0xFFFF >> mask[c]));
    if (c == 3)
        return upper ? magic[upper] : 2 + magic[lower];
    return upper ? clz2(upper, c + 1) : (16 >> (c)) + clz2(lower, c + 1);
}

static inline int clz64(uint64_t x)
{
    /* If the high 32 bits are nonzero, count within them.
     * Otherwise, count in the low 32 bits and add 32.
     */
    return (x >> 32) ? clz32((uint32_t) (x >> 32)) : clz32((uint32_t) x) + 32;
}

uint64_t sqrti(uint64_t x)
{
    uint64_t m, y = 0;
    if (x <= 1)
        return x;

    int total_bits = 64;

    /* clz64(x) returns the count of leading zeros in x.
     * (total_bits - 1 - clz64(x)) gives the index of the highest set bit.
     * Rounding that index down to an even number ensures our starting m is a
     * power of 4.
     */
    int shift = (total_bits - 1 - clz64(x)) & -2;
    m = 1ULL << shift;

    while (m) {
        uint64_t b = y + m;
        y >>= 1;
        if (x >= b) {
            x -= b;
            y += m;
        }
        m >>= 2;
    }
    return y;
}


int main(int argc, char **argv)
{
    u8 buf[512];
    size_t sz = sizeof(buf);
    int fd = open("/dev/random", O_RDONLY);
    assert(sz == read(fd, buf, sz));
    u64 *pbuf = (u64 *) buf;

    for (size_t i = 0; i < N; i++) {
        u64 v64 = *(u64 *) pbuf;
        u32 v32 = v64 & 0xffffffff;
        assert(__builtin_clz(v32) == clz32(v32));
        assert(__builtin_clzl(v64) == clz64(v64));

        assert((u64) sqrt(v64) == sqrti(v64));

        pbuf += 1;
        if (i && !((i + 1) & 63)) {
            read(fd, buf, sz);
            pbuf = (u64 *) buf;
        }
    }

    close(fd);
    return 0;
}