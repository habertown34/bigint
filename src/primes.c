#include <stdio.h>
#include <stdlib.h>

#include "bigint.h"

typedef struct _factors {
	int count;
	bigint* numbers;
} *factors;

factors primeFactors(bigint);

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Please input a number\n");
		exit(EXIT_FAILURE);
	}
	bigint n = newBIFromString(argv[1]);
	factors f = primeFactors(n);
	int i;
	for (i = 0; i < f->count; i++) {
		char* s = BItoString(f->numbers[i]);
		printf("%s ", s);
		free(s);
		deleteBI(f->numbers[i]);
	}
	printf("\n");
	free(f->numbers);
	free(f);
	deleteBI(n);

	return 0;
}

/* Returns prime factors of n */
factors primeFactors(bigint n) {
	bigint *res = (bigint*) malloc(sizeof(bigint));
	int count = 0;

	bigint d = newBIFromInt(2);
	bigint dSquared = multiplyBI(d, d);
	bigint zero = newBIFromInt(0);
	bigint one = newBIFromInt(1);
	bigint _n = copyBI(n);

	while(compareBI(_n, dSquared) >= 0) {
		deleteBI(dSquared);
		div_bigint div = divideBI(_n, d);
		if (compareBI(div->remainder, zero) == 0) {
			count++;
			res = (bigint*) realloc(res, count * sizeof(bigint));
			res[count - 1] = copyBI(d);
			deleteBI(_n);
			_n = div->quotient;
		} else {
			deleteBI(div->quotient);
			d = addBI(d, one);
		}
		deleteBI(div->remainder);
		free(div);
		dSquared = multiplyBI(d, d);
	}
	deleteBI(dSquared);

	count++;
	res = (bigint*) realloc(res, count * sizeof(bigint));
	res[count - 1] = _n;

	deleteBI(d);
	deleteBI(zero);
	deleteBI(one);

	factors fac = (factors) malloc(sizeof(struct _factors));
	fac->count = count;
	fac->numbers = res;

	return fac;
}