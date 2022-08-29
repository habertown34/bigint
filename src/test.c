
#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

int main()
{
    bigint b = newBigInteger("100102305");

    char* s = BItoString(b);

    printf("%s\n", s);
    
    return 0;
}
