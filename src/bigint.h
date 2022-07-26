#include <limits.h>

typedef struct
{
    int size; // in sizeof(int)
    unsigned int *data;
} BigInteger;

BigInteger copyBI(BigInteger* b);
void deleteBI(BigInteger* b);
void printBIData(BigInteger* b);
BigInteger* resizeBI(BigInteger* b, int s);
BigInteger* addBI(BigInteger* a, BigInteger* b);
BigInteger* negateBI(BigInteger* b);
BigInteger* subtractBI(BigInteger* a, BigInteger* b);