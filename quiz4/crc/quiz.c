#include <stdint.h>
#include <stdio.h>

static const uint32_t crc32c_table[256] = {
    0x00000000L, 0xF26B8303L, 0xE13B70F7L, 0x1350F3F4L,
    /* ... */
    0xBE2DA0A5L, 0x4C4623A6L, 0x5F16D052L, 0xAD7D5351L
};

uint32_t crc32_naive(uint32_t crc, uint8_t v)
{
    crc ^= v;
    for (int bit = 0; bit < 8; bit++) {
        if (crc & 1)
            crc = (crc >> 1) ^ UINT32_C(0x82f63b78);
        else
            crc = (crc >> 1);
    }
    return crc;
}

uint32_t crc32c(uint32_t crc, const uint8_t *data, unsigned int length)
{
    while (length--) {
        crc = crc32c_table[(crc ^ *data++) & 0xFFL] ^ (crc >> 8);
    }
    return crc ^ 0xffffffff;
}

/* CRC32C polynomial */
#define POLY  0xEDB88320
void generate() {
    printf("static const uint32_t crc32_table[16] = {\n");
    for (size_t i = 0; i < 16; i++) {
        uint32_t crc = i;
        for (uint32_t j = 0; j < 4; j++)
            crc = (crc >> 1) ^ (-(int)(crc & 1) & POLY);
        printf("    0x%08X%s", crc, (i % 4 == 3) ? ",\n" : ", ");
    }

    printf("};\n");
}

uint32_t crc32_u8(uint32_t crc, uint8_t v)
{
    crc ^= v;
    static const uint32_t crc32_table[] = {
        0x00000000, 0x105ec76f, 0x20bd8ede, 0x30e349b1,
        0x417b1dbc, 0x5125dad3, 0x61c69362, 0x7198540d,
        0x82f63b78, 0x92a8fc17, 0xa24bb5a6, 0xb21572c9,
        AAAA,       BBBB,       CCCC,       DDDD,
    };

    crc = (crc >> 4) ^ crc32_table[crc & 0x0F];
    crc = (crc >> 4) ^ crc32_table[crc & 0x0F];
    return crc;
}


int main(int argc, char **argv)
{

    generate();
    return 0;
}