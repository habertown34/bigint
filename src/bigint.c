#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "bigint.h"

// Assumes little endian
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    puts("");
}

void printBIData(BigInteger* b)
{
    printf("Size: %d Bits: ", b->size);
    printBits(b->size * sizeof(unsigned int), b->data);
}

int msbIsOneInt(unsigned int n)
{
    unsigned int mask = 1 << (8 * sizeof(unsigned int) - 1); // first bit 1, all other bits 0
    return (mask & n);
}

int msbIsOneBI(BigInteger* b)
{
    return msbIsOneInt(b->data[b->size - 1]);
}

int times10(int num)
{
    return (num << 1) + (num << 3);
}

int negate(int num)
{
    return (~num) + 1;
}

int max(int a, int b)
{
    if (a > b) return a;
    else return b;
}

BigInteger copyBI(BigInteger* b)
{
    BigInteger n;
    n.size = b->size;
    n.data = (unsigned int*) malloc(n.size * sizeof(unsigned int));
    if (n.data == NULL)
    {
        printf("Error: Can't allocate memory for copyBI\n");
        exit(EXIT_FAILURE);
    }
    int i;
    for(i = 0; i < n.size; i++)
    {
        n.data[i] = b->data[i];
    }
    return n;
}

void deleteBI(BigInteger* b)
{
    free(b->data);
}

BigInteger* resizeBI(BigInteger* b, int s)
{
    int isNegative = msbIsOneBI(b);
    int oldSize = b->size;
    if (s < oldSize)
    {
        printf("Error: reSize works only with bigger values");
        exit(EXIT_FAILURE);
    }
    b->size = s;
    b->data = realloc(b->data, s * sizeof(unsigned int));
    
    int i;
    for(i = oldSize; i < s; i++) // initialize new memory
    {
        if (isNegative)
            b->data[i] = UINT_MAX;
        else
            b->data[i] = 0;
    }
    
    return b;
}

BigInteger* negateBI(BigInteger* b);

/*
* What we need to do:
* 1) check if same size
* 2) if not, bring both to the same size
* 3) add them together, starting from the lowest bit
* 4) overflow handling
*/
BigInteger* addBI(BigInteger* a, BigInteger* b) // add b to a
{
    int aIsNegative = msbIsOneBI(a);
    int bIsNegative = msbIsOneBI(b);

    int size_max = max(a->size, b->size);
    if (a->size != size_max)
    {
        a->data = realloc(a->data, size_max * (sizeof(unsigned int)));
        if (a->data == NULL)
        {
            printf("Error: Can't realloc memory for a in addBI\n");
            exit(EXIT_FAILURE);
        }
        
        int i;
        for(i = (a->size); i < size_max; i++) // initialize new bytes
        {
            (a->data)[i] = 0;
        }
        
        a->size = size_max;
    }
    
    BigInteger cb = copyBI(b);
    BigInteger *c = &cb;
    if (c->size != size_max)
    {
        c->data = realloc(c->data, size_max * (sizeof(unsigned int)));
        if (c->data == NULL)
        {
            printf("Error: Can't realloc memory for c in addBI\n");
            exit(EXIT_FAILURE);
        }
        
        int i;
        for(i = (c->size); i < size_max; i++) // initialize new bytes
        {
            (c->data)[i] = 0;
        }
        
        c->size = size_max;
    }    
    
    //now the adding
    int i;
    int overflow = 0;
    int oldOverflow = 0;
    for(i = 0; i < size_max; i++)
    {
        if (UINT_MAX - a->data[i] < c->data[i])
        {
            overflow = 1;
        }
        else
        {
            overflow = 0;
        }
        a->data[i] += c->data[i];
        if (oldOverflow)
        {
            if(a->data[i] == UINT_MAX)
            {
                overflow = 1;
            }
            a->data[i] += 1;
        }
    }
    /*
     * If a or b (only one of them) is negative, then we need to check if we can reduce the number instead of checking for overflow
     */
    if ((aIsNegative && !bIsNegative) || (!aIsNegative && bIsNegative))
    {
        ; // TODO
    }
    else
    {
        int resultIsNegative = msbIsOneBI(a);
        if(!aIsNegative && resultIsNegative)
        {
            //positive overflow
            a->size += 1;
            a->data = realloc(a->data, a->size - 1);
            if (a->data == NULL)
            {
                printf("Error: Reallocationg a in a positive overflow");
                exit(EXIT_FAILURE);
            }
            a->data[a->size - 1] = 0;
        }
        if(aIsNegative && !resultIsNegative)
        {
            //negative overflow
            a->size += 1;
            a->data = realloc(a->data, a->size - 1);
            if (a->data == NULL)
            {
                printf("Error: Reallocationg a in a positive overflow");
                exit(EXIT_FAILURE);
            }
            a->data[a->size - 1] = UINT_MAX;            
        }
    }
    deleteBI(c);
    return a;
}

BigInteger* leftShiftBI(BigInteger* b, int amount)
{
    if (amount < 0)
    {
        printf("Error: Amount is negative\n");
        exit(EXIT_FAILURE);
    }
    int i;
    for(i = 0; i < amount; i++)
    {
        int j;
        int overflow = 0;
        int oldOverflow = 0;
        int currSize = b->size;
        for(j = 0; j < currSize; j++)
        {
            overflow = msbIsOneInt(b->data[j]);
            b->data[j] = b->data[j] << 1;
            if (oldOverflow) // if previous bits created an overflow, add 1 to this 
            {
                b->data[j] += 1;
            }
            oldOverflow = overflow;
            
            // if this is the highest part bits of the number, and there is an overflow
            // add new number
            if ((j == b->size - 1) && overflow)
            {
                ;
            }
        }
    }
    //TODO
    
    return b;
}

