#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"

/* Small helper function,
 * returns max of a and b */
int max(int a, int b) {
	if (a > b) return a;
	else return b;
}

struct bigInteger {
	int size; // in sizeof(int)
	unsigned int *data;
};

typedef struct _div_bigint_10 {
	bigint quotient;
	int remainder;
} *div_bigint_10;

/* Factory function
 * Allocates new bigint without initializing and returns pointer */
bigint newBigint() {
	bigint b = (bigint) malloc(sizeof(struct bigInteger));
	if (b == NULL) {
		printf("Error: Can't allocate memory for new bigint\n");
		return NULL;
	}
	b->size = 1;
	b->data = (unsigned int *) malloc(sizeof(unsigned int));
	if (b->data == NULL) {
		printf("Error: Can't allocate memory for new bigint\n");
		return NULL;
	}

	return b;
}

/* Factory function
 * Returns bigint with value of i */
bigint newBigintFromInt(int i) {
	bigint b = newBigint();
	b->data[0] = i;
	return b;
}

/* Prints the value as bits
 * Assumes little endian */
void printBits(size_t const size, void const * const ptr) {
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

/* Print data of bigint */
void printBIData(bigint b)
{
	printf("Size: %d Bits: ", b->size);
	printBits(b->size * sizeof(unsigned int), b->data);
}

/* Checks if most significant bit is 1 */
int msbIsOneInt(unsigned int n)
{
	unsigned int mask = 1 << (8 * sizeof(unsigned int) - 1); // first bit 1, all other bits 0
	return (mask & n);
}

/* Checks if most significant bit is 1 */
int msbIsOneBI(bigint b)
{
	return msbIsOneInt(b->data[b->size - 1]);
}

/* Clone bigint and return the clone */
bigint copyBI(bigint b) {
	bigint n = (bigint) malloc(sizeof(struct bigInteger));
	if (n == NULL) {
		printf("Error: Can't allocate memory for copyBI\n");
		return NULL;
	}
	n->size = b->size;
	n->data = (unsigned int*) malloc(n->size * sizeof(unsigned int));
	if (n->data == NULL) {
		printf("Error: Can't allocate memory for copyBI\n");
		return NULL;
	}
	int i;
	for (i = 0; i < n->size; i++) {
		n->data[i] = b->data[i];
	}
	return n;
}

/* Deallocate a bigint */
void deleteBI(bigint b) {
	free(b->data);
	free(b);
}

/* Returns 1 if a > b
 * 0 if equal
 * and -1 if a < b */
int compareBI(bigint a, bigint b) {
	int aIsNegative = msbIsOneBI(a);
	int bIsNegative = msbIsOneBI(b);

	if (!aIsNegative && bIsNegative)
		return 1;
	if (aIsNegative && !bIsNegative)
		return -1;

	if (aIsNegative) {    
		if (a->size != b->size) {
			if (a->size > b->size)
				return -1;
			else
				return 1;
		}
	} else {
		if (a->size != b->size) {
			if (a->size > b->size)
				return 1;
			else
				return -1;
		}    
	}

	int i;
	for (i = a->size - 1; i >= 0; i--) {
		if (a->data[i] > b->data[i]) {
			if (aIsNegative) 
				return -1;
			else
				return 1;
		}

		if (a->data[i] < b->data[i]) {
			if (aIsNegative) 
				return 1;
			else
				return -1;
		}
	}

	return 0;
}

bigint trimBI(bigint b)
{
	int isNegative = msbIsOneBI(b);
	int oldSize = b->size;
	int newSize = b->size;
	int i;

	if (!isNegative)
	{
		for(i = oldSize - 1; i > 0; i--)
		{
			if (b->data[i] == 0 && !msbIsOneInt(b->data[i-1]))
			{
				newSize -= 1;
			}
		}
	}
	else // b is negative
	{
		for(i = oldSize - 1; i > 0; i--)
		{
			if (b->data[i] == UINT_MAX && msbIsOneInt(b->data[i-1]))
			{
				newSize -= 1;
			}
		}
	}

	if (newSize != oldSize)
	{
		b->data = realloc(b->data, newSize * sizeof(unsigned int));
		if (b->data == NULL)
		{
			printf("Error: Reallocation in trimBI failed\n");
			exit(EXIT_FAILURE);
		}
		b->size = newSize;
	}

	return b;
}

bigint resizeBI(bigint b, int s)
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

/* Negates b */
bigint negateBI(bigint b)
{
	int i;
	for(i = 0; i < b->size; i++)
	{
		b->data[i] = ~(b->data[i]);

	}

	struct bigInteger one;
	one.size = 1;
	one.data = (unsigned int*) malloc(sizeof(unsigned int));
	one.data[0] = 1;
	b = addBI(b, &one);
	free(one.data);
	b = trimBI(b);

	return b;
}

/*
* What we need to do:
* 1) check if same size
* 2) if not, bring both to the same size
* 3) add them together, starting from the lowest bit
* 4) overflow handling
*/
bigint addBI(bigint a, bigint b) // add b to a
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
	
	bigint c = copyBI(b);
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
		oldOverflow = overflow;
	}
	/*
	 * If a or b (only one of them) is negative, then we need to check if we can reduce the number instead of checking for overflow
	 */
	if ((aIsNegative && !bIsNegative) || (!aIsNegative && bIsNegative))
	{
		a = trimBI(a);
	}
	else
	{
		int resultIsNegative = msbIsOneBI(a);
		if(!aIsNegative && resultIsNegative)
		{
			//positive overflow
			a->size += 1;
			a->data = realloc(a->data, a->size * sizeof(unsigned int));
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
			a->data = realloc(a->data, a->size  * sizeof(unsigned int));
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

/* Subtracts b from a and returns a */
bigint subtractBI(bigint a, bigint b) {
	bigint c = copyBI(b);
	c = negateBI(c);
	a = addBI(a, c);
	deleteBI(c);
	return a;
}

bigint leftShiftBI(bigint b, int amount)
{
	if (amount < 0)
	{
		printf("Error: Amount is negative\n");
		exit(EXIT_FAILURE);
	}
	int i;
	int bIsNegative = msbIsOneBI(b);
	if (bIsNegative)
	{ // left shift only makes sense on positive Integers
		b = negateBI(b);
	}
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
			if (j == b->size - 1)
			{
				overflow = msbIsOneInt(b->data[j]);
				if (overflow)
				{
					//positive overflow
					b->size += 1;
					b->data = realloc(b->data, b->size  * sizeof(unsigned int));
					if (b->data == NULL)
					{
						printf("Error: Reallocationg b in a positive overflow in leftSHift");
						exit(EXIT_FAILURE);
					}
					b->data[b->size - 1] = 0;
				}
			}
		}
	}
	if (bIsNegative)
	{
		b = negateBI(b);
	}
	
	return b;
}

bigint times10(bigint b)
{
	bigint a = copyBI(b);
	a = leftShiftBI(a, 1);
	b = leftShiftBI(b, 3);
	b = addBI(b, a);
	deleteBI(a);
	return b;
}

/* Returns new allocated bigint from string */
bigint newBigIntFromString(const char* str) {
	int l = strlen(str);
	//printf("l = %d\n", l);
	int negative = (str[0] == '-');
	//printf("%d\n", negative);
	if (negative)
	{
		str++;
		l -= 1;
	}

	bigint b = newBigint();
	b->data[0] = 0;
	
	struct bigInteger d;
	d.size = 1;
	d.data = (unsigned int*) malloc(sizeof(unsigned int));
	int i;
	char n;
	char c;
	for (i = 0; i < l; i++) {
		c = str[i];
		//printf("Char = %c\n", c);
		n = c - '0';
		//printf("Charn = %d\n", (unsigned int) n);
		d.data[0] = (unsigned int) n;
		b = times10(b);
		//printBIData(b);
		b = addBI(b, &d);
		//printBIData(b);
	}
	free(d.data);

	if (negative)
	{
		b = negateBI(b);
	}
	
	return b;
}

/* Divide with remainder */
div_bigint divideBI(bigint a, bigint b) {
	if (b->size == 1 && b->data[0] == 0)
	{
		printf("Error: Division by zero\n");
		exit(EXIT_FAILURE);
	}
	int aIsNegative = msbIsOneBI(a);
	int bIsNegative = msbIsOneBI(b);
	
	if (aIsNegative) {
		a = negateBI(a);
	}
	if (bIsNegative) {
		b = negateBI(b);
	}    
	
	bigint remainder = copyBI(a);
	bigint quotient = newBigint();
	quotient->data[0] = 0;

	bigint one = newBigint();
	one->data[0] = 1;

	while(compareBI(remainder, b) >= 0) {
		subtractBI(remainder, b);
		addBI(quotient, one);
	}

	if ((aIsNegative && !bIsNegative) || (!aIsNegative && bIsNegative)) {
		quotient = negateBI(quotient);
	}

	if (bIsNegative) {
		remainder = negateBI(remainder);
	}

	if (aIsNegative) {
		a = negateBI(a);
	}
	if (bIsNegative) {
		b = negateBI(b);
	}    
	deleteBI(one);

	div_bigint result = (div_bigint) malloc(sizeof(struct _div_bigint));
	result->quotient = quotient;
	result->remainder = remainder;
	
	return result;
}

/* Divide bigint by ten */
div_bigint_10 divideBI10(bigint b) {
	bigint quotient = copyBI(b);
	int remainder = 0;

	int negative = msbIsOneBI(b);
	if (negative) {
		quotient = negateBI(quotient);
	}

	long helper;
	int i;
	for (i = b->size - 1; i >= 0; i--) {
		helper = ((long) remainder * (1L << (8 * sizeof(unsigned int))) + b->data[i]);
		quotient->data[i] = (int) (helper / 10);
		remainder = (int) (helper % 10);
	}

	if (negative) {
		quotient = negateBI(quotient);
		remainder = 0 - remainder;
	}

	quotient = trimBI(quotient);

	div_bigint_10 result = (div_bigint_10) malloc(sizeof(struct _div_bigint_10));
	result->quotient = quotient;
	result->remainder = remainder;
	return result;
}

/* Return a decimal representation of b */
char* BItoString(bigint b) {
	int bIsNegative = msbIsOneBI(b);

	if (bIsNegative) {
		negateBI(b);
	}

	char *buf = (char *) malloc(12 * (b->size));

	bigint ten = newBigint();
	ten->data[0] = 10;

	char *p = buf + 12 * (b->size) - 1;
	*p = '\0';
	p--;
	
	bigint quotient;
	int remainder;
	bigint dividend = copyBI(b);
	div_bigint_10 divResult;

	while(compareBI(dividend, ten) >= 0) {
		divResult = divideBI10(dividend);
		quotient = divResult->quotient;
		remainder = divResult->remainder;
		free((void*) divResult);
		//printf("rem = %d\n", remainder);

		deleteBI(dividend);
		//printBIData(quotient);
		dividend = quotient;
		
		*p = '0' + remainder;
		p--;
	}

	int div = dividend->data[0];
	*p = '0' + div;

	deleteBI(ten);
	deleteBI(dividend);

	if (bIsNegative) {
		p--;
		*p = '-';
	}

	char *str = (char*) malloc(strlen(p) + 1);
	strcpy(str, p);
	free((void*) buf);

	return str;
}

/* Returns result of multiplication */
bigint multiplyBI(bigint a, bigint b) {
	int aIsNegative = msbIsOneBI(a);
	int bIsNegative = msbIsOneBI(b);

	if (aIsNegative) {
		a = negateBI(a);
	}
	if (bIsNegative) {
		b = negateBI(b);
	}

	bigint result = newBigintFromInt(0);

	int mask;
	int i, j;
	for (i = 0; i < b->size; i++) {
		for (j = 0; j <= 8 * sizeof(unsigned int) - 1; j++) {
			mask = 1 << j;
			if (mask & b->data[i]) {
				bigint adder = copyBI(a);
				adder = leftShiftBI(adder, 32 * i + j);
				result = addBI(result, adder);
				deleteBI(adder);
			}
		}
	}

	if (aIsNegative) {
		a = negateBI(a);
	}
	if (bIsNegative) {
		b = negateBI(b);
	}
	if ((aIsNegative && !bIsNegative) || (!aIsNegative && bIsNegative)) {
		result = negateBI(result);
	}

	return result;
}