#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

void printUsage(const char *argv0);

int main(int argc, char *argv[])
{
	int newline = 1;
	int append = 0;

	int c; while ((c = getopt(argc, argv, "an")) != -1) {
		switch (c) {
		case 'a':
			append = 1;
			break;
		case 'n':
			newline = 0;
			break;
		case '?':
			fprintf(stderr, "%s: invalid option -%c\n", argv[0], optopt);
			printUsage(argv[0]);
			return 2;
		}
	}

	if (argc - optind < 1) {
		printUsage(argv[0]);
		return 2;
	}

	FILE *fp = fopen(argv[optind], append ? "a" : "w");
	if (!fp) {
		fprintf(stderr, "%s: %s: %s\n", argv[0], argv[optind], strerror(errno));
		return 1;
	}

	int i; for (i=optind+1; i<argc; ++i) {
		fprintf(fp, "%s", argv[i]);
		if (i < argc-1)
			fputc(' ', fp);
	}

	if (newline)
		fputc('\n', fp);
	
	fclose(fp);
	return 0;
}

void printUsage(const char *argv0)
{
	fprintf(stderr, "Usage: %s [-an] filename [string]...\n", argv0);
	fprintf(stderr, "Opens a file and writes a string.\n");
	fprintf(stderr, "\t-a\tOpen file in append mode (do not truncate)\n");
	fprintf(stderr, "\t-n\tDo not add newline\n");
}
