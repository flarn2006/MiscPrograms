#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *catArgs(int argc, char *argv[])
{
	size_t length = 0;
	int i; for (i=1; i<argc; i++) {
		length += strlen(argv[i]);
		if (i < argc - 1) length++; /* space */
	}

	char *buf = malloc(length + 1);
	*buf = '\0';
	for (i=1; i<argc; i++) {
		strcat(buf, argv[i]);
		if (i < argc - 1) strcat(buf, " ");
	}

	return buf;
}

int main(int argc, char *argv[]) 
{
	char *ptr, *buf;
	unsigned int runlength = 0;
	
	if (argc < 2) {
		printf("Usage: %s string\n", argv[0]);
		return 1;
	}

	ptr = buf = catArgs(argc, argv);
	
	while (*ptr != '\0') {
		static char last;

		if (ptr == buf) /*if first character*/ last = *ptr;
		
		if (*ptr == last) runlength ++;
		else {
			printf("%u\tx '%c'\n", runlength, last);
			runlength = 1;
			last = *ptr;
		}

		ptr++;
	}

	printf("%u\tx '%c'\n", runlength, *(ptr-1));

	free(buf);
	return 0;
}
