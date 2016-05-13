#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getValue(int *ar, size_t count, size_t pos);

int main(int argc, char *argv[])
{
	/* Process arguments */
	int rows = 10;
	int width = 4;
	if (argc > 1) {
		rows = atoi(argv[1]);
		if (argc > 2) {
			width = atoi(argv[2]);
		}
	}
	if (rows < 1 || width < 2) {
		fprintf(stderr, "Usage: %s [number-of-rows] [width]\n", argv[0]);
		fprintf(stderr, "* number-of-rows, if present, must be at least 1.\n");
		fprintf(stderr, "* width, if present, must be at least 2.\n");
		fprintf(stderr, "  - For best results, use an even number.");
		return 255;
	}

	/* Set up the arrays */
	int *row = malloc(sizeof(int) * rows);
	int *prev = malloc(sizeof(int) * rows);
	row[0] = 1;
	prev[0] = 1;

	/* Display Pascal's triangle */
	int i; for (i=0; i<rows; ++i) {
		int j;
		/* Display the current row */
		for (j=0; j<(rows - i - 1); ++j) {
			int k; for (k=0; k < (width + 1) / 2; ++k) {
				putchar(' ');
			}
		}
		for (j=0; j<=i; ++j) {
			printf("%*d ", width - 1, row[j]);
		}
		puts("");

		/* Calculate the next row */
		if (i < rows - 1) {
			for (j=0; j<=i+1; ++j) {
				row[j] = getValue(prev, i+1, j) + getValue(prev, i+1, j-1);
			}
			memcpy(prev, row, sizeof(int) * (i + 2));
		}
	}
	
	/* Free allocated memory */
	free(row);
	free(prev);

	return 0;
}

int getValue(int *ar, size_t count, size_t pos)
{
	if (pos < 0 || pos >= count) return 0;
	else return ar[pos];
}
