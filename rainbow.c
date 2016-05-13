#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const char *usage_str = "Usage: %s [filename]\n";
const char *ansi_color_code_fmt = "\x1B[%um";
const char *ansi_color_reset_str = "\x1B[m";

typedef enum { RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA } ansi_color;
typedef unsigned int ansi_color_code;

ansi_color_code nextANSIColorCode(ansi_color *color)
{
	ansi_color_code colorCodes[] = { 31, 33, 32, 36, 34, 35 };
	ansi_color_code codeToReturn = colorCodes[*color];
	*color = (*color + 1) % 6;
	return codeToReturn;
}

int main(int argc, char *argv[])
{
	FILE *fp;
	
	/* Process the program's arguments */
	if (argc == 2) {
		fp = fopen(argv[1], "r");
		if (!fp) {
			perror(argv[0]);
			return 1;
		}
	} else if (argc == 1) {
		fp = stdin;
	} else {
		printf(usage_str, argv[0]);
		return 2;
	}

	/* Loop through the file */
	for (;;) { /* Loop ends when fgetc(fp) returns EOF */
		static ansi_color color = RED;
		int ch = fgetc(fp);
		if (ch == EOF) break;
		printf(ansi_color_code_fmt, nextANSIColorCode(&color));
		putchar(ch);
		if (ch == '\n') {
			printf("%s", ansi_color_reset_str);
			color = RED;
		}
	}

	printf("%s", ansi_color_reset_str);
	if (fp != stdin) fclose(fp);

	return 0;
}
