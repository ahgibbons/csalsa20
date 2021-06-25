#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include "salsafuncs.h"

#define NONCEWORDS 2
#define KEYWORDS 8
#define COUNTERWORDS 2
#define NONCEBYTES (NONCEWORDS * 4)
#define KEYBYTES (KEYWORDS * 4)
#define COUNTERBYTES (COUNTERWORDS * 4)
#define R(W,A) ((W << A) | (W >> (32-A)) )

// Big Endian
const uint32_t A0 = 0x65787061;
const uint32_t A1 = 0x6e642033;
const uint32_t A2 = 0x322d6279;
const uint32_t A3 = 0x7465206b;


uint8_t testkey[KEYWORDS*4] = { 1, 2, 3, 4, 5, 6, 7, 8,
                    9,10,11,12,13,14,15,16,
                   17,18,19,20,21,22,23,24,
                   25,26,27,28,29,30,31,32};

uint8_t testnonce[NONCEWORDS*4] = {3,1,4,1,5,9,2,6};

uint8_t testpos[COUNTERWORDS*4]   = {7,0,0,0,0,0,0,0};



uint32_t littleendian(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
    uint32_t w;
    w = (uint32_t)b0 + ((uint32_t)b1<<8) + ((uint32_t)b2<<16) + ((uint32_t)b3<<24);
    printf(" %d %d %d %d \t\t", b0,b1,b2,b3);
    printf("0x%x\n",w);
    return w;
}

void bs2words(uint8_t *bs, uint32_t *ws, int bslen) {
    if (bslen%4 != 0) {
        printf("Bytestring will not fit into an even number of words. Exit\n");
        exit(0);
    }
    uint32_t tempw;
    for (size_t i = 0; i < bslen/4; i++)
    {
        tempw = littleendian(bs[4*i], bs[4*i+1], bs[4*i+2], bs[4*i+3]);
        ws[i]=tempw;
    }
    
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



void expansion32(uint8_t k[32], uint8_t n[16]) {
    uint8_t k0[16];
    uint8_t k1[16];
    uint8_t input[64];

    for (int i = 0; i < 16; i++) {
       k0[i] = k[i];
       k1[i] = k[i+16];
    }

    

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

void printwordstring(uint32_t *bs, int length) {
    for (size_t i = 0; i < length; i++)
    {
        printf("0x%x\n", bs[i]);
    }
    
}

int main(int argc, char const *argv[])
{
    uint32_t k[KEYWORDS];
    uint32_t nonce[NONCEWORDS];
    uint32_t counter[COUNTERWORDS];
    
    bs2words(testkey, k, KEYBYTES);
    bs2words(testnonce, nonce, NONCEBYTES);
    bs2words(testpos, counter, COUNTERBYTES);

    printwordstring(k, KEYWORDS);
    printf("\n");
    printwordstring(nonce, NONCEWORDS);
    printf("\n");
    printwordstring(counter, COUNTERWORDS);
    printf("\n\n");

    

    return 0;
}