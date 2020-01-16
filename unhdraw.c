#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *fp = NULL;
	const char *filename = "standard input";

	if (argc < 2) {
		fp = stdin;
	} else if (!strcmp(argv[1], "--help")) {
		fprintf(stderr, "Usage: %s [ BYTES... | -f FILENAME ]\n");
		return 0;
	} else if (!strcmp(argv[1], "-f")) {
		if (argc == 2) {
			fprintf(stderr, "%s: -f requires a filename as an argument.\n", argv[0]);
			return 255;
		} else if (argc == 3) {
			filename = argv[2];
			fp = fopen(filename, "r");
			if (!fp) {
				perror(filename);
				return 2;
			}
		} else {
			fprintf(stderr, "%s: too many arguments after -f\n", argv[0]);
			return 255;
		}
	} else {
		fprintf(stderr, "%s: reading hex on cmdline is not yet implemented\n", argv[0]);
		/* TODO: implement it */
		return 254;
	}

	for (;;) {
		int byte;
		int scanf_result = fscanf(fp, "%2x", &byte);
		if (scanf_result == EOF) {
			if (ferror(fp))
				perror(filename);
			break;
		} else if (scanf_result == 0) {
			fgetc(fp);
		} else {
			putchar(byte);
		}
	}

	if (fp && fp != stdin) {
		fclose(fp);
	}

	return 0;
}
