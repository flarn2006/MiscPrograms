/* exclude.c - Print some args without certain values
 * Useful for shell wildcards.
 * Usage: exclude [args...] [- [blacklist...]]
 *   e.g. exclude ab cd ef gh ij - cd gh  ->  ab ef ij
 * Example: Move all files except for .jp(e)g files to a directory
 *   mv `exclude "*" - "*.jpg" "*.jpeg"` non_jpegs/
 * NOTE: Quoting wildcards is important because of the way `` works!
 *
 * WARNING: DO NOT use `exclude` for anything (like rm) that can't be undone!
 *          I have encountered issues before where it doesn't exclude like it
 *          should, and I haven't figured out what the cause is yet.
 */

#warning "[1;5;7;33m READ THE COMMENT AT THE TOP OF EXCLUDE.C BEFORE USING IT! [m"
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
