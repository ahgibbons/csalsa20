#ifndef SALSA20UTILS
#define SALSA20UTILS


uint32_t littleendian(uint8_t, uint8_t, uint8_t, uint8_t);

void bs2words(const uint8_t *bs, uint32_t *ws, int bslen);

void printblock_as_words(const uint8_t block[64]);

void QR(uint32_t x[], 
            int a, int b, int c, int d);

void rowround(uint32_t x[16]);

void colround(uint32_t x[16]);

void doubleround(uint32_t x[16]);

void printblock(uint32_t block[16]);

void show_mem_rep(char *start, int n);

#endif