
#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

int main()
{
    BigInteger b;
    BigInteger* p = &b;
    //b.size = 2;
    //b.data = (unsigned int*) malloc((b.size)* sizeof(unsigned int));
    //b.data[0] = 0b11111111111111111111111111111010;
    //b.data[1] = 0;
    
    BigInteger b2;
    BigInteger* p2 = &b2;
    b2.size = 2;
    b2.data = (unsigned int*) malloc((b2.size)* sizeof(unsigned int));
    b2.data[0] = 6;
    b2.data[1] = 0;
    //printBIData(p);
    //printBIData(p2);
    p = newBigInteger(p, "32000000000000000000");
    printBIData(p);
    //printBIData(p2);
    return 0;
}
