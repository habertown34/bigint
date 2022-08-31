
#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

int main() {
	bigint b = newBIFromString("-36");
	bigint c = newBIFromString("-36");
	bigint d = multiplyBI(b, c);
	bigint e = newBIFromString("-2");
	bigint f = multiplyBI(d,e);
	char *s = BItoString(f);
	printf("%s\n", s);

	deleteBI(b);
	deleteBI(c);
	deleteBI(d);
	deleteBI(e);
	deleteBI(f);
	free(s);
	
	return 0;
}
