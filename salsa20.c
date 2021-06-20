#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define R(W,A) ((W << A) | (W >> (32-A)) )
)

// Big Endian
const uint32_t A0 = 0x65787061;
const uint32_t A1 = 0x6e642033;
const uint32_t A2 = 0x322d6279;
const uint32_t A3 = 0x7465206b;

uint32_t iblock[16] = {
    A0, 0, 0, 0,
     0,A1, 0, 0,
     0, 0,A2, 0,
     0, 0, 0,A3
}

void initblock(uint32_t k[8], uint32_t nonce[2], uint32_t counter[2], uint32_t out[16]) {
    out[0]  = A0;
    out[5]  = A1;
    out[10] = A2;
    out[15] = A3;

    
}

void expand_key(uint32_t out[16], const uint32_t in[16]) {
    
}


void show_mem_rep(char *start, int n) {
    for (int i = 0; i < n; i++)
    {
        printf(" %.2x", start[i]);
    }
    printf("\n");
}


inline void QR(uint32_t x[], 
            int a, int b, int c, int d) {
    x[b] ^= R(x[a] + x[d], 7);
    x[c] ^= R(x[b] + x[a], 9);
    x[d] ^= R(x[c] + x[b], 13);
    x[a] ^= R(x[d] + x[c], 18);
    }

inline void rowround(uint32_t x[16]) {
    QR(x, 0,1,2,3);
    QR(x, 5,6,7,4);
    QR(x, 10,11,8,9);
    QR(x, 15,12,13,14);
}

inline void colround(uint32_t x[16]) {
    QR(x, 0,4,8,12);
    QR(x, 5,9,13,1);
    QR(x, 10,14,2,6);
    QR(x, 15,3,7,11);
}

inline void doubleround(uint32_t x[16]) {
    colround(x);
    rowround(x);
}

uint32_t littleendian(uint32_t w) {
    return ((w & 0xff000000) >> 24) + 
           ((w & 0x00ff0000) >> 8) +
           ((w & 0x0000ff00) << 8) +
           ((w & 0x000000ff) << 24); 
}

void salsa20hash(uint32_t w[16]) {
    uint32_t x[16];
    for (size_t i = 0; i < 16; i++)
    {
        w[i] = littleendian(w[i]);
        x[i] = w[i];
    }
    

    for (size_t i = 0; i < 10; i++)
    {
        doubleround(x);
    }
    
    for (size_t i = 0; i < 16; i++)
    {
        w[i] = littleendian(x[0]+w[0]);
    }
}

void expansion32(uint32_t k[8], uint32_t n[4]) {
    uint32_t k0[4], k1[4];
    for (size_t i = 0; i < 4; i++)
    {
        k0[i] = k[i];
        k1[i] = k[i+4];
    }

    uint32_t a0,a1,a2,a3;
    a0 = 0x65787061;
    a1 = 0x6e642033;
    a2 = 0x322d6279;
    a3 = 0x7465206b;

    uint32_t input[16];
    input[0] = a0;
    input[5] = a1;
    input[10] = a2;
    input[15] = a3;
    for (size_t i = 0; i < 4; i++)
    {
        input[1+i] = k0[i];
        input[6+i] = n[i];
        input[11+i] = k1[i];
    }    

    salsa20hash(input);
}


uint8_t *wordtoba(uint32_t w) {
    uint8_t *ba;
    ba = malloc(sizeof(*ba) * 4);
    ba[0] = (w & 0xff000000) >> 24;
    ba[1] = (w & 0x00ff0000) >> 16;
    ba[2] = (w & 0x0000ff00) >> 8;
    ba[3] = (w & 0x000000ff);

    return ba;
}

int main(int argc, char const *argv[])
{
    uint32_t w = 0x12345678;
    uint8_t *ba = wordtoba(w);
    for (size_t i = 0; i < 4; i++)
    {
        printf("%x ", ba[i]);
    }
    printf("\n");

    int i = 0x12345678;
    show_mem_rep((char *)&i, sizeof(i));
    
    return 0;
}