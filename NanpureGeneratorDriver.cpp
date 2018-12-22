#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "NanpureGenerator.h"
#include "NanpureBoard.h"

int rng(int range) {
	if (range <= 0) {
		puts("rng error: range <= 0");
		exit(1);
	}
	return rand() % range;
}

int main(int argc, char* argv[]) {
	unsigned int seed = (unsigned int)time(NULL);
	if (argc > 1) sscanf(argv[1], "%u", &seed);
	printf("seed = %u\n", seed);
	srand(seed);
	NanpureBoard nb = generateNanpure(rng);
	for (int i = 0; i < 9; i++) {
		printf("%d", nb.getNumber(i, 0));
		for (int j = 1; j < 9; j++) {
			printf(" %d", nb.getNumber(i, j));
		}
		putchar('\n');
	}
	return 0;
}
