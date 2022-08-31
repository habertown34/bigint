
#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

int main() {
	bigint b = newBIFromString("3432379232");
	bigint c = newBIFromString("2");
	div_bigint d = divideBI(b, c);
	bigint quot = d.quotient;
	bigint rem = d.remainder;
	char *sq = BItoString(quot);
	char *sr = BItoString(rem);
	printf("%s %s\n", sq, sr);

	bigint f = newBIFromString("2147483648");
	printf("%s\n", BItoString(f));
	f = negateBI(f);
	printf("%s\n", BItoString(f));
	b = addBI(b, f);
	char *sb = BItoString(b);
	printf("%s\n", sb);
	free(sb);
	deleteBI(f);

	deleteBI(b);
	deleteBI(c);
	deleteBI(quot);
	deleteBI(rem);
	free(sq);
	free(sr);
	
	return 0;
}
