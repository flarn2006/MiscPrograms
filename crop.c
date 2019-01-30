#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { BEFORE_PREFIX, AT_TARGET, AFTER_SUFFIX } state_t;

int main(int argc, char *argv[])
{
	const char *filename;
	FILE *fp;
	if (argc == 4) {
		filename = argv[3];
		fp = fopen(filename, "r");
		if (!fp) {
			perror(filename);
			return 1;
		}
	} else if (argc == 3) {
		filename = "standard input";
		fp = stdin;
	} else {
		fprintf(stderr, "Usage: %s PREFIX SUFFIX [FILENAME]\n", argv[0]);
		return 255;
	}

	const char *prefix = argv[1];
	const char *suffix = argv[2];
	size_t prefix_len = strlen(prefix);

	size_t bufsize = 1024;
	char *buf = malloc(bufsize);
	state_t state = BEFORE_PREFIX;

	while (!feof(fp)) {
		fgets(buf, bufsize, fp);
		size_t len = strlen(buf);
		while (len == bufsize-1 && buf[len-1] != '\n') {
			size_t old_bufsize = bufsize;
			bufsize *= 2;
			buf = realloc(buf, bufsize);
			fgets(buf+len-1, old_bufsize, fp);
			len = strlen(buf);
		}

		const char *pfxstart = strstr(buf, prefix);
		if (pfxstart) {
			const char *tgtstart = pfxstart + prefix_len;
			size_t tgt_len;
			const char *sfxstart = strstr(tgtstart, suffix);
			int already_did_newline = 0;
			if (sfxstart) {
				tgt_len = sfxstart - tgtstart;
			} else {
				tgt_len = len - (tgtstart - buf);
				already_did_newline = 1;
			}
			fwrite(tgtstart, 1, tgt_len, stdout);
			if (!already_did_newline)
				putchar('\n');
		}
	}

	fclose(fp);
	free(buf);
	return 0;
}
