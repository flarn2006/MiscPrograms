#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "delayms.h"

void printUsage(FILE *stream, const char *argv0);

int main(int argc, char *argv[])
{
	int ms = 50;
	int bytes = 1;
	FILE *fp = stdin;

	if (argc >= 5) {
		printUsage(stderr, argv[0]);
		return 2;
	}

	if (argc >= 2) {
		if (strcmp(argv[1], "--help") == 0) {
			printUsage(stdout, argv[0]);
			return 0;
		} else {
			ms = atoi(argv[1]);
		}
	}

	if (argc >= 3) {
		bytes = atoi(argv[2]);
	}
	
	if (argc == 4) {
		fp = fopen(argv[3], "r");
		if (!fp) {
			fprintf(stderr, "%s: error opening '%s': %s\n", argv[0], argv[3], strerror(errno));
			return 1;
		}
	}

	while (1) {
		int i; for (i=0; i<bytes; ++i) {
			int ch = fgetc(fp);
			if (feof(fp)) {
				break;
			} else {
				putchar(ch);
			}
		}
		fflush(stdout);
		delayms(ms);
	}

	if (fp != stdin) fclose(fp);
	return 0;
}

void printUsage(FILE *stream, const char *argv0)
{
	fprintf(stream, "Usage: %s [milliseconds between steps] [bytes to output per step] [filename]\n", argv0);
}
