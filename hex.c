#include <stdio.h>
#include <limits.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s number\n", argv[0]);
		return 2;
	}

	long long n;
	int scanf_result = sscanf(argv[1], "%lld", &n);
	
	if (scanf_result == 0 || scanf_result == EOF) {
		fprintf(stderr, "%s: invalid number '%s'\n", argv[0], argv[1]);
		return 1;
	}

	unsigned long long u = LLONG_MAX;
	sscanf(argv[1], "%llu", &u);
	printf("%llx\n", (n == LLONG_MAX) ? u : n);

	return 0;
}
