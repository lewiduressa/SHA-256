/*
  author: Lewi Duressa
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

const int H[8] = {0x6a09e667, 0xbb67ae85,
                  0x3c6ef372, 0xa54ff53a,
                  0x510e527f, 0x9b05688c,
                  0x1f83d9ab, 0x5be0cd19};

const int K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

void printBin(char *ptr)
{
    for (int i = 0; i < 8; i++)
    {
        printf("%d", !!((*ptr << i) & 0x80));
    }
}

void printBinInt(uint32_t num) {
    // Number of bits in an unsigned int
    int bits = 32;

    // Iterate over each bit
    for (int i = bits - 1; i >= 0; i--) {
        // Print the bit at the current position
        printf("%d", (num >> i) & 1);
        if(i%8==0) {
            printf(" ");
        }
    }
}

uint32_t addB(uint32_t a, uint32_t b) {
    return (a+b)%0x100000000;
}

void printArr(char *data, int size, uint32_t *dataInt, int sizeInt)
{
    if (data != NULL)
    {
        char *a = data;
        for (int i = 0; i < size; i++)
        {
            if (i % 4 == 0)
            {
                printf("\n");
            }
            if(i<10) {
                printf(" %d  ", i);
            } else {
                printf("%d  ", i);
            }
            printBin(a);
            printf(" ");
            a++;
        }
        printf("\n");
    }

    if (dataInt != NULL)
    {
        uint32_t *a = dataInt;
        for (int i = 0; i < sizeInt; i++)
        {
            printf("\n");
            if(i<10) {
                printf(" %d  ", i);
            } else {
                printf("%d  ", i);
            }
            printBinInt(*a);
            a++;
        }
        printf("\n");
    }
}

void zeroOutArr(char *ptr, uint32_t *ptrInt, int size, int sizeI)
{
    if (ptr != NULL)
    {
        for (int i = 0; i < size; i++)
        {
            ptr[i] = 0;
        }
    }
    if (ptrInt != NULL)
    {
        for (int i = 0; i < sizeI; i++)
        {
            ptrInt[i] = 0;
        }
    }
}

int rightRotate(uint32_t x, int n)
{
    int shifted = x >> n;
    int rot_bits = x << (32 - n);
    int combined = shifted | rot_bits;

    return combined;
}

uint32_t leftRotate(uint32_t x, int n)
{
    uint32_t shifted = x << n;
    uint32_t rot_bits = x >> (32 - n);
    uint32_t combined = shifted | rot_bits;
    return combined;
}

uint32_t sigma0(uint32_t n)
{
    uint32_t r1 = rightRotate(n, 7);
    uint32_t r2 = rightRotate(n, 18);
    uint32_t r3 = n >> 3;

    return r1^r2^r3;
}

uint32_t sigma1(uint32_t n)
{
    uint32_t r1 = rightRotate(n, 17);
    uint32_t r2 = rightRotate(n, 19);
    uint32_t r3 = n >> 10;

    return r1^r2^r3;
}

uint32_t BigSigma0(uint32_t n) {
    uint32_t r1 = rightRotate(n, 2);
    uint32_t r2 = rightRotate(n, 13);
    uint32_t r3 = rightRotate(n, 22);

    return r1^r2^r3;
}

uint32_t BigSigma1(uint32_t n) {
    uint32_t r1 = rightRotate(n, 6);
    uint32_t r2 = rightRotate(n, 11);
    uint32_t r3 = rightRotate(n, 25);

    return r1^r2^r3;
}

void populateMessageBlocks(unsigned char *inputC, uint32_t *message, uint64_t bits, int mlen)
{
    inputC[(int)(bits / 8.0)] = 0x80;
    int len = (int)(bits / 32)+1;
    for (int i = 0; i < len; i++)
    {
        uint32_t a = (uint32_t)inputC[i * 4] << 24;
        uint32_t b = (uint32_t)inputC[(i * 4) + 1] << 16;
        uint32_t c = (uint32_t)inputC[(i * 4) + 2] << 8;
        uint32_t d = (uint32_t)inputC[(i * 4) + 3];
        message[i] |= a | b | c | d;
    }
    uint32_t a = (uint32_t)(bits >> 32);
    uint32_t b = (uint32_t)(bits);
    message[mlen - 2] = a;
    message[mlen - 1] = b;
}

void populateW(uint32_t *W, uint32_t *message) {
    for(int i=0; i<16;i++) {
        W[i] = message[i];
    }
    
    for(int i=16; i<64;i++) {
        W[i] = addB(addB(addB(sigma1(W[i-2]), W[i-7]), sigma0(W[i-15])), W[i-16]);
    }
}

uint32_t Ch(uint32_t a, uint32_t b, uint32_t c) {
    uint32_t d = 0;
    for(int i=0;i<32;i++) {
        if((a>>i)<<31 != 0) {
            if((b>>i)<<31 != 0) {
                d |= 0x00000001 << i;
            }
        } else {
            if((c>>i)<<31 != 0) {
                d |= 0x00000001 << i;
            }
        }
    }

    return d;
}

uint32_t Maj(uint32_t a, uint32_t b, uint32_t c) {
    uint32_t d = 0;
    int count1 = 0;
    for(int i=0;i<32;i++) {
        count1 = 0;
        if((a>>i)<<31 != 0) {
            count1++;
        }
        if((b>>i)<<31 != 0) {
            count1++;
        }
        if((c>>i)<<31 != 0) {
            count1++;
        }
        if(count1>=2) {
            d |= 0x00000001 << i;
        } 
    }

    return d;
}

void hash(uint32_t *W, uint32_t *AInit, uint32_t *hashCode) {
    uint32_t *A = malloc(32*8);
    for(int i=0;i<8;i++) {
        A[i]=AInit[i];
    }
    for(int i=0;i<64;i++) {
        uint32_t T1 = addB(addB(addB(addB(A[7], BigSigma1(A[4])), Ch(A[4], A[5], A[6])), K[i]),  W[i]);
        uint32_t T2 = addB(BigSigma0(A[0]), Maj(A[0], A[1], A[2]));
        A[7] = A[6];
        A[6] = A[5];
        A[5] = A[4];
        A[4] = addB(A[3], T1);
        A[3] = A[2];
        A[2] = A[1];
        A[1] = A[0];
        A[0] = addB(T1, T2);
    }
    for(int i=0;i<8;i++) {
        hashCode[i]=addB(AInit[i], A[i]);
    }
    free(A);
}

void printHash(uint32_t *hashCode) {
    for(int i=0;i<8;i++) {
        printf("%x",hashCode[i]);
    }
    printf("\n");
}

int main()
{
    const int WORD_SIZE = 256;
    char inputC[WORD_SIZE];
    zeroOutArr(inputC, NULL, WORD_SIZE, 0);
    scanf("%s", inputC);

    uint64_t bits = strlen((char *)inputC) * 8;
    int messageArrBits = 512 + 512 * ((bits / 512)) + ((int)(fmod(bits, 512) + 1) / 448) * 512;
    int numBlocks = messageArrBits / 512;
    int mlen = messageArrBits / 32;
    uint32_t message[mlen];
    zeroOutArr(NULL, message, 0, mlen);
    populateMessageBlocks((unsigned char*)inputC, message, bits, mlen);
    //printArr(NULL, 0, message, mlen);

    uint32_t W[numBlocks][64];
    uint32_t A[numBlocks][8];
    uint32_t *hashCode = malloc(32*8);

    for(int i=0;i<8;i++) {
        A[0][i]= H[i];
    }

    for (int i = 0; i < numBlocks; i++)
    {
        populateW(W[i], message+i*16);
        //printArr(NULL, 0, W[i], 64);
        hash(W[i], A[i], hashCode);
        if(i+1<numBlocks) {
            for(int j=0;j<8;j++) {
                A[i+1][j] = hashCode[j];
            }
        }
        
    }

    printHash(hashCode); // r: 454349...
    free(hashCode);

    return 0;
}
