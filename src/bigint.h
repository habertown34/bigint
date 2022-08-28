#include <limits.h>

extern struct bigInteger;
typedef struct bigInteger *bigint;

bigint newBigint(void);
bigint copyBI(bigint b);
void deleteBI(bigint b);
void printBIData(bigint b);
bigint resizeBI(bigint b, int s);
bigint addBI(bigint a, bigint b);
bigint negateBI(bigint b);
bigint subtractBI(bigint a, bigint b);
bigint leftShiftBI(bigint b, int amount);
bigint times10(bigint b);
bigint newBigInteger(bigint b, const char* str);
