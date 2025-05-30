#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "common.h"


#define POLY 0xEDB88320
/* CRC32C polynomial */
#define POLY 0x82F63B78

void generate_crc32c_table() {
    printf("static const uint32_t crc32c_table[16] = {\n");
    for (uint32_t i = 0; i < 16; ++i) {
        uint32_t crc = i;
        for (int j = 0; j < 4; ++j) {
            if (crc & 1)
                crc = (crc >> 1) ^ POLY;
            else
                crc = (crc >> 1);
        }
        printf("    0x%08x%s", crc, (i % 4 == 3) ? ",\n" : ", ");
    }
    printf("};\n");


}


void dump_table(uint32_t *tlb, int sz) {
    for (int i = 0; i < sz; i++) {
        printf("%08x ", tlb[i]);

        if ((i + 1) % 4 == 0) {
            puts("");
        }
    }
}

void generate()
#if 0
{
    printf("static const uint32_t crc32_table[16] = {\n");
    for (size_t i = 0; i < 16; i++) {
        uint32_t crc = i;
        for (uint32_t j = 0; j < 4; j++)
            crc = (crc >> 1) ^ (-(int) (crc & 1) & POLY);
        printf("    0x%08X%s", crc, (i % 4 == 3) ? ",\n" : ", ");
    }

    printf("};\n");
}
#else
{
    u32 tlb[16] = {0};
    for (unsigned int i = 0; i < 16; i++)
    {
      unsigned int crc = i;
      for (unsigned int j = 0; j < 4; j++)
        crc = (crc >> 1) ^ (-(int)(crc & 1) & POLY);
      tlb[i] = crc;
    }
    dump_table(tlb, 16);

}
#endif

uint32_t crc32_u8_v2(uint32_t crc, uint8_t v)
{
    static const uint32_t crc32_table[] = {
        0x00000000, 0x105ec76f, 0x20bd8ede, 0x30e349b1,
        0x417b1dbc, 0x5125dad3, 0x61c69362, 0x7198540d,
        0x82f63b78, 0x92a8fc17, 0xa24bb5a6, 0xb21572c9,
        0xc38d26c4, 0xd3d3e1ab, 0xe330a81a, 0xf36e6f75,
    };

    // method2
    crc = crc32_table[(crc ^  v      ) & 0x0F] ^ (crc >> 4);
    crc = crc32_table[(crc ^ (v >> 4)) & 0x0F] ^ (crc >> 4);
    return crc;
}
uint32_t crc32_u8(uint32_t crc, uint8_t v)
{
    static const uint32_t crc32_table[] = {
        0x00000000, 0x105ec76f, 0x20bd8ede, 0x30e349b1,
        0x417b1dbc, 0x5125dad3, 0x61c69362, 0x7198540d,
        0x82f63b78, 0x92a8fc17, 0xa24bb5a6, 0xb21572c9,
        0xc38d26c4, 0xd3d3e1ab, 0xe330a81a, 0xf36e6f75,
    };

    // method1
    crc ^= v;
    crc = (crc >> 4) ^ crc32_table[crc & 0x0F];
    crc = (crc >> 4) ^ crc32_table[crc & 0x0F];


    return crc;
}

uint32_t crc32_opt_v2(uint8_t *data, size_t sz)
{
    uint32_t crc = ~0;
    for (size_t i = 0; i < sz; i++) {
        crc = crc32_u8_v2(crc, data[i]);
    }
    return ~crc;
}

uint32_t crc32_opt(uint8_t *data, size_t sz)
{
    uint32_t crc = ~0;
    for (size_t i = 0; i < sz; i++) {
        crc = crc32_u8(crc, data[i]);
    }
    return ~crc;
}

uint32_t crc32_bitwise(const void *data, size_t length, uint32_t poly)
{
    uint32_t crc = ~0; // same as previousCrc32 ^ 0xFFFFFFFF
    unsigned char *current = (unsigned char *) data;
    while (length--) {
        crc ^= *current++;
        for (unsigned int j = 0; j < 8; j++)
            if (crc & 1)
                crc = (crc >> 1) ^ poly;
            else
                crc = crc >> 1;
    }

    return ~crc;  // same as crc ^ 0xFFFFFFFF
}

void gen_crc_tlb(uint32_t poly)
{
    uint32_t Crc32Lookup[256] = {0};
    for (unsigned int i = 0; i <= 0xFF; i++) {
        uint32_t crc = i;
        for (unsigned int j = 0; j < 8; j++)
            crc = (crc >> 1) ^ (-((int)(crc & 1)) & poly);
        Crc32Lookup[i] = crc;
    }
    dump_table(Crc32Lookup, 256);
}

uint32_t crc32(uint8_t *data, size_t sz, uint32_t poly)
{
    uint32_t crc = 0;
    // crc;
    for (size_t i = 0; i < sz; i++) {
        uint8_t byte = data[i];

        crc ^= (uint32_t) (byte << 24);

        for (int j = 0; j < 8; j++) {
            if (crc & 0x80000000) {
                crc = (uint32_t) ((crc << 1) ^ poly);
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}

uint32_t bit_reverse(uint32_t x) {
    uint32_t n = x;
    n = ((n & 0xffff0000) >> 16) | ((n & 0x0000ffff) << 16);
    n = ((n & 0xff00ff00) >>  8) | ((n & 0x00ff00ff) <<  8);
    n = ((n & 0xf0f0f0f0) >>  4) | ((n & 0x0f0f0f0f) <<  4);
    n = ((n & 0xcccccccc) >>  2) | ((n & 0x33333333) <<  2);
    n = ((n & 0xaaaaaaaa) >>  1) | ((n & 0x55555555) <<  1);
    return n;
}

int main(int argc, char **argv)
{
    uint8_t data[] = {
        0x23, 0x47, 0x85, 0x32, 0x04, 0x78, 0x59, 0x32,
        0x47, 0x05, 0x83, 0x27, 0x46, 0x95, 0x81, 0xcf,
        0x43,0x98,0x75,0x67,0x23,0x98,0x47,0x56,0x87,
        0x21,
        0x34,0x25,0x68,0x97,0x42,0x35,0x69,0x82,0x34,0x52
    };
    size_t sz = ARRAY_SIZE(data);
    uint32_t poly = 0x105ec76f;
    uint32_t c = crc32(data, sz, poly);
    uint32_t k = bit_reverse(c);
    // printf("%x, %x\n", crc32_opt(data, sz), crc32_bitwise(data, sz, 0x82f63b78));
    // printf("%x, %x\n", crc32_opt(data, sz), crc32_opt_v2(data, sz));

    // CRC(A ^ B) == CRC(A) ^ CRC(B) ？
    uint32_t a = crc32_u8(0, data[0]);
    uint32_t b = crc32_u8(0, data[1]);
    uint32_t cr = crc32_u8(0 , data[0] ^ data[1]);
    printf("%x,%x,%x\n", a,b,cr);
    printf("%x\n", a^b);


    // printf("%x\n", crc_opt2);
    // printf("%x\n", k);
    // gen_crc_tlb(poly);
    // generate_crc32c_table();
    // generate();


    return 0;
}