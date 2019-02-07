#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int exit_status = 0;
	const char *filename;
	FILE *fp;
	if (argc == 4) {
		filename = argv[3];
		fp = fopen(filename, "r");
		if (!fp) {
			fprintf(stderr, "%s: cannot access '%s': %s\n", argv[0], filename, strerror(errno));
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
	if (!buf) {
		fprintf(stderr, "%s: Out of memory\n", argv[0]);
		exit_status = 2;
		goto err_close_file;
	}

	setbuf(stdout, NULL);  /* for easier error handling */

	while (!feof(fp)) {
		if (fgets(buf, bufsize, fp) == NULL) {
fgets_null_handler:
			if (errno) {
				fprintf(stderr, "%s: error reading '%s': %s\n", argv[0], filename, strerror(errno));
				exit_status = 1;
				goto err_free_and_close;
			} else {
				break;
			}
		}
		size_t len = strlen(buf);
		while (len == bufsize-1 && buf[len-1] != '\n') {
			size_t old_bufsize = bufsize;
			bufsize *= 2;
			buf = realloc(buf, bufsize);
			if (!buf) {
				fprintf(stderr, "%s: Out of memory\n", argv[0]);
				exit_status = 2;
				goto err_free_and_close;
			}
			if (fgets(buf+len-1, old_bufsize, fp) == NULL)
				goto fgets_null_handler;  /* It's okay, don't worry. Dinosaurs are extinct. */
			len = strlen(buf);
		}

		const char *pfxstart = strstr(buf, prefix);
		if (pfxstart) {
			const char *tgtstart = pfxstart + prefix_len;
			size_t tgt_len;
			const char *sfxstart = NULL;
			if (*suffix)
				sfxstart = strstr(tgtstart, suffix);
			
			int already_did_newline = 0;
			if (sfxstart) {
				tgt_len = sfxstart - tgtstart;
			} else {
				tgt_len = len - (tgtstart - buf);
				already_did_newline = 1;
			}
			if (fwrite(tgtstart, 1, tgt_len, stdout) < tgt_len) {
				fprintf(stderr, "%s: write error: %s\n", argv[0], strerror(errno));
				return 1;
			} else if (!already_did_newline) {
				putchar('\n');
			}
		}
	}

err_free_and_close:
	free(buf);
err_close_file:
	if (fp != stdin)
		fclose(fp);
	return exit_status;
}
