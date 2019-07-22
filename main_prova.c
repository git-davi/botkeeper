#include <stdio.h>
#include "util.h"

#define _CRT_SECURE_NO_WARNINGS

int main(void){
	vector_t p1 = {0, 1};
	vector_t p2 = {3, 1};

	retta_t r = retta_due_punti(p1, p2);

	printf("la retta ottenuta è %dx + %dy + %d\n", r.a, r.b, r.c);

	vector_t p = {2, 3};
	printf("la distanza è %f\n", dist_punto_retta(p, r));

	int x = -5;

	x = x / abs(x);

	printf("x e' %d\n", x);

	return EXIT_SUCCESS;
}
