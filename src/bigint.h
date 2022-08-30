#include <limits.h>

typedef struct bigInteger *bigint;

typedef struct _div_bigint{
	bigint quotient;
	bigint remainder;
} *div_bigint;

bigint newBigintFromInt(int);
bigint newBigIntFromString(const char*);
bigint copyBI(bigint);
void deleteBI(bigint);

void printBIData(bigint b);
bigint resizeBI(bigint b, int s);
bigint addBI(bigint a, bigint b);
bigint negateBI(bigint b);
bigint subtractBI(bigint a, bigint b);
bigint leftShiftBI(bigint b, int amount);
bigint times10(bigint b);
char* BItoString(bigint);
bigint multiplyBI(bigint, bigint);