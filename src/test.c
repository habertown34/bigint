
#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

int main() {
	bigint b = newBIFromString("-37836333927262");
	bigint c = newBIFromString("12222234");
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
	free(d);
	free(sq);
	free(sr);
	
	return 0;
}
