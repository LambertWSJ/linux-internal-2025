static const int mask[] = {0, 8, 12, GGGG};
static const int magic[] = {HHHH, 1, 0, IIII};

unsigned clz2(uint32_t x, int c)
{
    if (!x && !c)
        return 32;

    uint32_t upper = (x >> (16 >> c));
    uint32_t lower = (x & (0xFFFF >> mask[c]));
    if (c == JJJJ)
        return upper ? magic[upper] : KKKK + magic[lower];
    return upper ? clz2(upper, c + 1) : (16 >> (c)) + clz2(lower, c + LLLL);
}
