#ifndef _BIGINT_H_
#define _BIGINT_H_

typedef struct bigInteger *bigint;

typedef struct {
	bigint quotient;
	bigint remainder;
} div_bigint;

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
div_bigint divideBI(bigint, bigint);

#endif