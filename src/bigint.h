#include <limits.h>

typedef struct bigInteger *bigint;

typedef struct _div_bigint{
	bigint quotient;
	bigint remainder;
} *div_bigint;

bigint newBIFromInt(int);
bigint newBIFromString(const char*);
char* BItoString(bigint);
bigint copyBI(bigint);
void deleteBI(bigint);

int compareBI(bigint, bigint);
bigint negateBI(bigint);
bigint addBI(bigint, bigint);
bigint subtractBI(bigint, bigint);
bigint multiplyBI(bigint, bigint);