#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "salsafuncs.h"
#include "salsa20const.h"



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