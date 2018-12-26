#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cinttypes>
#include "NanpureGenerator.h"
#include "NanpureBoard.h"

uint32_t x = 123456789;
uint32_t y = 362436069;
uint32_t z = 521288629;
uint32_t w = 88675123;

uint32_t xorshift(void) {
	uint32_t t = x ^ (x << 11);
	x = y;
	y = z;
	z = w;
	w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
	return w;
}

void xorshift_seed(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
	if ((a|b|c|d) != 0) {
		x = a;
		y = b;
		z = c;
		w = d;
		// reduce direct influence of seed values
		for (int i = 0; i < 100; i++) xorshift();
	}
}

int rng(int range) {
	if (range <= 0) {
		puts("rng error: range <= 0");
		exit(1);
	}
	uint32_t range_u = range;
	uint32_t rest = UINT32_MAX % range_u;
	uint32_t result;
	do {
		result = xorshift();
	} while (result < rest);
	return result % range_u;
}

enum output_format_t {
	NORMAL,
	ONELINE
};

int main(int argc, char* argv[]) {
	uint32_t a = 0, b = 0, c = 0, d  = 0;
	bool seed_set = false;
	output_format_t output_format = NORMAL;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--seed") == 0 || strcmp(argv[i], "-s") == 0) {
			if (++i < argc) {
				if (sscanf(argv[i], "%" SCNx32 "-%" SCNx32 "-%" SCNx32 "-%" SCNx32, &a, &b, &c, &d) != 4 || (a|b|c|d) == 0) {
					puts("invalid seed");
					return 1;
				}
				seed_set = true;
			} else {
				puts("no seed value");
				return 1;
			}
		} else if (strcmp(argv[i], "--outformat") == 0 || strcmp(argv[i], "-f") == 0) {
			if (++i < argc) {
				if (strcmp(argv[i], "normal") == 0) output_format = NORMAL;
				else if (strcmp(argv[i], "oneline") == 0) output_format = ONELINE;
				else {
					puts("unknown output format");
					return 1;
				}
			} else {
				puts("no output format");
				return 1;
			}
		}
	}
	if (!seed_set) {
		uint32_t t = (uint32_t)time(NULL);
		xorshift_seed(t, t + 1, t + 2, t + 3);
		do {
			a = xorshift();
			b = xorshift();
			c = xorshift();
			d = xorshift();
		} while ((a|b|c|d) == 0);
	}
	xorshift_seed(a, b, c, d);
	char seedString[64];
	snprintf(seedString, sizeof(seedString),
		"seed = %08" PRIX32 "-%08" PRIX32 "-%08" PRIX32 "-%08" PRIX32, a, b, c, d);
	NanpureBoard nb = generateNanpure(rng);
	switch (output_format) {
		case NORMAL:
			printf("%s\n\n", seedString);
			for (int i = 0; i < 9; i++) {
				printf("%d", nb.getNumber(i, 0));
				for (int j = 1; j < 9; j++) {
					printf(" %d", nb.getNumber(i, j));
				}
				putchar('\n');
			}
			break;
		case ONELINE:
			printf("%s\n\n", seedString);
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					printf("%d", nb.getNumber(i, j));
				}
			}
			putchar('\n');
			break;
	}
	return 0;
}
