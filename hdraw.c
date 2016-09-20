/* hdraw.c - Convert data to a raw hex dump              */
/* Usage: hdraw [filename | -]...                        */
/* Reads from stdin by default, or when '-' is specified */

#include <stdio.h>
#include <string.h>
#include <errno.h>

char hexdigit(int value)
{
	value &= 15;
	if (value < 10) return '0'+value;
	else return 'a'+(value-10);
}

int dumpfile(const char *path)
{
	FILE *fp;
	if (path == NULL || strcmp(path, "-") == 0) {
		fp = stdin;
	} else {
		fp = fopen(path, "r");
		if (!fp) return 0;
	}

	while (1) {
		int ch = fgetc(fp);
		if (feof(fp)) {
			break;
		} else {
			putchar(hexdigit(ch >> 4));
			putchar(hexdigit(ch));
		}
	}

	if (fp != stdin) fclose(fp);

	return 1;
}

int main(int argc, char *argv[])
{
	if (argc < 2) argc = 2; /* use the null terminator to mean stdin */

	int i; for (i=1; i<argc; ++i) {
		if (!dumpfile(argv[i])) {
			/* dumpfile won't return an error for stdin */
			fprintf(stderr, "%s: cannot open '%s': %s\n", argv[0], argv[i], strerror(errno));
			return 1;
		}
	}

	return 0;
}
