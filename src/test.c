
#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

int main() {
	bigint b = newBigintFromInt(87217780);
	bigint c = newBigIntFromString("375927823467392");
	bigint d = multiplyBI(b, c);

	//printBIData(b);

	char *s = BItoString(d);
	printf("%s\n", s);

	deleteBI(b);
	deleteBI(c);
	deleteBI(d);
	free(s);
	
	return 0;
}
