
#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

int main()
{
	bigint b = newBigInteger("-3");
	b = leftShiftBI(b, 3);

	printBIData(b);

	char *s = BItoString(b);
	printf("%s\n", s);

	deleteBI(b);
	free(s);
	
	return 0;
}
