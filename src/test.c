
#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

int main() {
	bigint b = newBIFromString("-227270");
	bigint c = newBIFromString("2233");
	div_bigint d = divideBI(b, c);
	bigint quot = d->quotient;
	bigint rem = d->remainder;
	char *sq = BItoString(quot);
	char *sr = BItoString(rem);
	printf("%s %s\n", sq, sr);

	deleteBI(b);
	deleteBI(c);
	deleteBI(quot);
	deleteBI(rem);
	free((void*) d);
	free((void*) sq);
	free((void*) sr);
	
	return 0;
}
