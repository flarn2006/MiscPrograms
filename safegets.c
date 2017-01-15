#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

size_t agets(char **str, size_t buf_size)
{
	/* NOTE: buf_size is the initial size, not the maximum. The buffer expands to fit input. */
	
	char *buf = malloc(buf_size);
	if (!buf) {
		*str = NULL;
		return (size_t)-1;
	}
	
	size_t length = 0;

	int error = 0;
	int ch; while ((ch = getchar()) != '\n') {
		if (length + 2 >= buf_size) {
			if (buf_size <= SIZE_MAX/2) {
				buf_size *= 2;
				char *newptr = realloc(buf, buf_size);
				if (newptr) {
					buf = newptr;
				} else {
					error = 1;
					break;
				}
			} else {
				error = 1;
				break;
			}
		}

		buf[length] = ch;
		++length;
	}

	if (error) {
		free(buf);
		*str = NULL;
		return (size_t)-1;
	} else {
		buf[length] = '\0';
		*str = buf;
		return length;
	}
}

int main(int argc, char *argv[])
{
	printf("Enter a string: ");
	
	char *str;
	size_t len = agets(&str, 64);

	if (str) {
		printf("You entered \"%s\".\n", str);
		printf("That contains %zu character(s).\n", len);
		free(str);
		return 0;
	} else {
		fprintf(stderr, "Error allocating memory.\n");
		return 1;
	}
}
