/* exclude.c - Print some args without certain values
 * Useful for shell wildcards.
 * Usage: exclude [args...] [- [blacklist...]]
 *   e.g. exclude ab cd ef gh ij - cd gh  ->  ab ef ij
 * Example: Delete all files except for .jp(e)g files
 *   rm `exclude * - *.jpg *.jpeg`
 */

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int dash = argc;

	int i;
	for (i=1; i<argc; ++i) {
		if (!strcmp(argv[i], "-")) {
			dash = i;
			break;
		}
	}

	for (i=1; i<dash; ++i) {
		int excluded = 0;
		int j; for (j=dash+1; j<argc; ++j) {
			if (!strcmp(argv[i], argv[j])) {
				excluded = 1;
				break;
			}
		}
		if (!excluded)
			puts(argv[i]);
	}

	return 0;
}
