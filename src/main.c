
#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

int main()
{
    BigInteger b;
    BigInteger* p = &b;
    b.size = 1;
    b.data = (unsigned int*) malloc((b.size)* sizeof(unsigned int));
    b.data[0] = 100;
    //b.data[1] = 0b01111111111111111111111111111111;
    
    BigInteger b2;
    BigInteger* p2 = &b2;
    b2.size = 1;
    b2.data = (unsigned int*) malloc((b2.size)* sizeof(unsigned int));
    b2.data[0] = 5;
    //b2.data[1] = 0b10000000000000000000000000000000;
    printBIData(p);
    printBIData(p2);    
    p = times10(p);
    printBIData(p);
    //printBIData(p2);
    return 0;
}
