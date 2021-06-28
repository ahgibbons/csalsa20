#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "salsafuncs.h"


#define R(W,A) ((W << A) | (W >> (32-A)) )

uint32_t littleendian(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
    uint32_t w;
    w = (uint32_t)b0 + ((uint32_t)b1<<8) + ((uint32_t)b2<<16) + ((uint32_t)b3<<24);
    return w;
};

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
    
};

inline void QR(uint32_t x[], 
            int a, int b, int c, int d) {
    x[b] ^= R(x[a] + x[d], 7);
    x[c] ^= R(x[b] + x[a], 9);
    x[d] ^= R(x[c] + x[b], 13);
    x[a] ^= R(x[d] + x[c], 18);
};

inline void rowround(uint32_t x[16]) {
    QR(x, 0,1,2,3);
    QR(x, 5,6,7,4);
    QR(x, 10,11,8,9);
    QR(x, 15,12,13,14);
};

inline void colround(uint32_t x[16]) {
    QR(x, 0,4,8,12);
    QR(x, 5,9,13,1);
    QR(x, 10,14,2,6);
    QR(x, 15,3,7,11);
};

void doubleround(uint32_t x[16]) {
    colround(x);
    rowround(x);
};

void printblock(uint32_t block[16]) {
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            printf("%08x ", block[i*4 +j]);
        }
        printf("\n");
        
    }
    
};

void show_mem_rep(char *start, int n) {
    for (int i = 0; i < n; i++)
    {
        printf(" %.2x", start[i]);
    }
    printf("\n");
};