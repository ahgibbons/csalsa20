#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "salsafuncs.h"
#include "salsa20const.h"

#define NONCEWORDS 2
#define KEYWORDS 8
#define COUNTERWORDS 2
#define NONCEBYTES (NONCEWORDS * 4)
#define KEYBYTES (KEYWORDS * 4)
#define COUNTERBYTES (COUNTERWORDS * 4)
#define BLOCKSIZE 64
#define BLOCKSIZEWORD (BLOCKSIZE/4)

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
}

void wordblock2bytes(uint32_t wblock[16], uint8_t bblock[64]) {
    uint8_t b0,b1,b2,b3;
    uint32_t w;
    for (size_t i = 0; i < 16; i++)
    {
        w = wblock[i];
        b0 = (uint8_t) w & 0xffffffff;
        b1 = (uint8_t) (w >> 8) & 0xffffffff;
        b2 = (uint8_t) (w >> 16) & 0xffffffff;
        b3 = (uint8_t) (w >> 24) & 0xffffffff;

        bblock[i*4]   = b0;
        bblock[i*4+1] = b1;
        bblock[i*4+2] = b2;
        bblock[i*4+3] = b3;
    }
    
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

void salsa20_block(uint8_t key8[KEYBYTES], uint8_t nonce8[NONCEBYTES],
                uint64_t count, uint8_t kstream[BLOCKSIZE]) {
    uint32_t k[KEYWORDS];
    uint32_t nonce[NONCEWORDS];
    uint32_t counter[COUNTERWORDS];

    bs2words(key8, k, KEYBYTES);
    bs2words(nonce8, nonce, NONCEBYTES);
    counter[0] = count & 0xffffffff;
    counter[1] = count >> 32;

    uint32_t iblock[16];
    uint32_t wblock[16];

    initblock(k,nonce,counter,iblock);
    salsaround(wblock,iblock);

    wordblock2bytes(wblock, kstream);
}


// Encrypts input string, test program
int main(int argc, char const *argv[])
{
    FILE *pfile; // Plaintext file
    FILE *ofile; // cipher text file
    FILE *kfile; // Key file
    FILE *nfile; // nonce file
    uint8_t buffer[BLOCKSIZE];
    size_t rnum;


    if (argc != 5) {
        printf("Incorrect number of args.\nUsage: ./salsa20 <input file> <key file> <nonce file> <output file>\n");
        exit(0);
    }

    if (!(pfile = fopen(argv[1], "rb"))) {
        printf("Error opening input file! Exit\n");
        exit(0);
    };
    if (!(kfile = fopen(argv[2], "rb"))) {
        printf("Error opening key file! Exit\n");
        exit(0);
    };
    if (!(nfile = fopen(argv[3], "rb"))) {
        printf("Error opening nonce file! Exit\n");
        exit(0);
    };
    if (!(ofile = fopen(argv[4], "wb"))) {
        printf("Error opening output file! Exit\n");
        exit(0);
    };

    uint8_t kstream[BLOCKSIZE];
    uint8_t cblock[BLOCKSIZE];
    uint64_t count = 0;
    
    uint8_t key[KEYBYTES];
    uint8_t nonce[NONCEBYTES];

    if ((rnum = fread(key, 1, KEYBYTES, kfile))!=KEYBYTES) {
        printf("Error. Key has incorrect size\n");
        exit(0);
    }

    if ((rnum = fread(nonce, 1, NONCEBYTES, nfile))!=NONCEBYTES) {
        printf("Error. Nonce has incorrect size\n");
        exit(0);
    }

    printf("Key: ");
    for (size_t i = 0; i < KEYBYTES; i++)
    {
        printf("%02x ", key[i]);
    }
    printf("\n");

    printf("None: ");
    for (size_t i = 0; i < NONCEBYTES; i++)
    {
        printf("%02x ", nonce[i]);
    }
    printf("\n");
    

    while ((rnum = fread(buffer, 1, BLOCKSIZE, pfile))) {
        
        salsa20_block(key, nonce, count, kstream);
        for (size_t i = 0; i < rnum; i++)
        {
            cblock[i] = buffer[i] ^ kstream[i];
        }
        fwrite(cblock, 1, rnum, ofile);

        count++;
    };
    
    fclose(pfile);
    fclose(ofile);
    fclose(nfile);
    fclose(kfile);

    return 0;
}