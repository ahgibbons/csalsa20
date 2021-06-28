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


// Test values
uint8_t testkey[KEYWORDS*4] = { 1, 2, 3, 4, 5, 6, 7, 8,
                    9,10,11,12,13,14,15,16,
                   17,18,19,20,21,22,23,24,
                   25,26,27,28,29,30,31,32};

uint8_t testnonce[NONCEWORDS*4] = {3,1,4,1,5,9,2,6};

uint8_t testpos[COUNTERWORDS*4]   = {7,0,0,0,0,0,0,0};

uint32_t testresult[16] = {0xb9a205a3, 0x0695e150, 0xaa94881a, 0xadb7b12c,
0x798942d4, 0x26107016, 0x64edb1a4, 0x2d27173f,
0xb1c7f1fa, 0x62066edc, 0xe035fa23, 0xc4496f04,
0x2131e6b3, 0x810bde28, 0xf62cb407, 0x6bdede3d};


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

    printf("Key: ");
    printwordstring(k, KEYWORDS);
    printf("\nNonce: ");
    printwordstring(nonce, NONCEWORDS);
    printf("\nCounter: ");
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