#ifndef SALSA20UTILS
#define SALSA20UTILS

#include "salsa20const.h"

void initblock(uint32_t k[KEYWORDS], uint32_t n[NONCEWORDS], 
            uint32_t c[COUNTERWORDS], uint32_t s[BLOCKSIZEWORD]);

uint32_t littleendian(uint8_t, uint8_t, uint8_t, uint8_t);

void bs2words(const uint8_t *bs, uint32_t *ws, int bslen);

void printblock_as_words(const uint8_t block[BLOCKSIZE]);

void QR(uint32_t x[], 
            int a, int b, int c, int d);

void rowround(uint32_t x[BLOCKSIZEWORD]);

void colround(uint32_t x[BLOCKSIZEWORD]);

void doubleround(uint32_t x[BLOCKSIZEWORD]);

void printblock(uint32_t block[BLOCKSIZEWORD]);


#endif