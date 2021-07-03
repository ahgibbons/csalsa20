#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "salsa20const.h"
#include "salsafuncs.h"


#define R(W,A) ((W << A) | (W >> (32-A)) )

// Big Endian
const uint32_t A0 = 0x61707865;
const uint32_t A1 = 0x3320646e;
const uint32_t A2 = 0x79622d32;
const uint32_t A3 = 0x6b206574;

// Create Initial block state in 's'
void initblock(uint32_t k[KEYWORDS], uint32_t n[NONCEWORDS], 
            uint32_t c[COUNTERWORDS], uint32_t s[16]) {
    s[0]  = A0;   //
    s[1]  = k[0];
    s[2]  = k[1];
    s[3]  = k[2];
    s[4]  = k[3]; //
    s[5]  = A1;
    s[6]  = n[0];
    s[7]  = n[1];
    s[8]  = c[0]; //
    s[9]  = c[1];
    s[10] = A2;
    s[11] = k[4];
    s[12] = k[5]; //
    s[13] = k[6];
    s[14] = k[7];
    s[15] = A3;

}

void printwordstring(uint32_t *bs, int length) {
    for (size_t i = 0; i < length; i++)
    {
        printf("0x%x\n", bs[i]);
    }
    
};

inline void word2bs(uint32_t w, uint8_t *bs) {
    uint8_t b0,b1,b2,b3;
    bs[0] = (uint8_t) w & 0xffffffff;
    bs[1] = (uint8_t) (w >> 8) & 0xffffffff;
    bs[2] = (uint8_t) (w >> 16) & 0xffffffff;
    bs[3] = (uint8_t) (w >> 24) & 0xffffffff;
};

uint32_t littleendian(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
    uint32_t w;
    w = (uint32_t)b0 + ((uint32_t)b1<<8) + ((uint32_t)b2<<16) + ((uint32_t)b3<<24);
    return w;
};

void bs2words(const uint8_t *bs, uint32_t *ws, int bslen) {
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
    
};

inline void QR(uint32_t x[], 
            int a, int b, int c, int d) {
    x[b] ^= R(x[a] + x[d], 7);
    x[c] ^= R(x[b] + x[a], 9);
    x[d] ^= R(x[c] + x[b], 13);
    x[a] ^= R(x[d] + x[c], 18);
};

inline void rowround(uint32_t x[BLOCKSIZEWORD]) {
    QR(x, 0,1,2,3);
    QR(x, 5,6,7,4);
    QR(x, 10,11,8,9);
    QR(x, 15,12,13,14);
};

inline void colround(uint32_t x[BLOCKSIZEWORD]) {
    QR(x, 0,4,8,12);
    QR(x, 5,9,13,1);
    QR(x, 10,14,2,6);
    QR(x, 15,3,7,11);
};

void doubleround(uint32_t x[BLOCKSIZEWORD]) {
    colround(x);
    rowround(x);
};

void printblock(uint32_t block[BLOCKSIZEWORD]) {
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            printf("0x%08x ", block[i*4 +j]);
        }
        printf("\n");
        
    }
    
};

void printblock_as_words(const uint8_t block[64]) {
    uint32_t pblock[BLOCKSIZEWORD];
    bs2words(block, pblock, BLOCKSIZE);
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            printf("0x%08x ", pblock[i*4 +j]);
        }
        printf("\n");
        
    }
    
};