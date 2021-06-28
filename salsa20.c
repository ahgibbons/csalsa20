#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "salsafuncs.h"

#define NONCEWORDS 2
#define KEYWORDS 8
#define COUNTERWORDS 2
#define NONCEBYTES (NONCEWORDS * 4)
#define KEYBYTES (KEYWORDS * 4)
#define COUNTERBYTES (COUNTERWORDS * 4)
#define R(W,A) ((W << A) | (W >> (32-A)) )

// Big Endian
const uint32_t A0 = 0x61707865;
const uint32_t A1 = 0x3320646e;
const uint32_t A2 = 0x79622d32;
const uint32_t A3 = 0x6b206574;


uint8_t testkey[KEYWORDS*4] = { 1, 2, 3, 4, 5, 6, 7, 8,
                    9,10,11,12,13,14,15,16,
                   17,18,19,20,21,22,23,24,
                   25,26,27,28,29,30,31,32};

uint8_t testnonce[NONCEWORDS*4] = {3,1,4,1,5,9,2,6};

uint8_t testpos[COUNTERWORDS*4]   = {7,0,0,0,0,0,0,0};





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
    
};

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



void salsaround(uint32_t out[16], const uint32_t in[16]) {
    uint32_t x[16];

    for (size_t i = 0; i < 16; i++)
    {
        x[i] = in[i];
    }
    

    for (size_t i = 0; i < 10; i++)
    {
        doubleround(x);
    }
    
    for (size_t i = 0; i < 16; i++)
    {
        out[i] = x[i] + in[i];
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

    uint32_t block[16];
    initblock(k,nonce,counter,block);

    printblock(block);

    printf("\n\n");
    uint32_t oblock[16];
    salsaround(oblock, block);

    printblock(oblock);
    
    return 0;
}