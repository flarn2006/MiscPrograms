#include <stdio.h>

int main(int argc, char *argv[]) 
{
	char *ptr;
	unsigned int runlength = 0;
	
	if (argc != 2) {
		printf("Usage: %s string\n", argv[0]);
		return 1;
	}

	ptr = argv[1];
	while (*ptr != '\0') {
		static char last;

		if (ptr == argv[1]) /*if first character*/ last = *ptr;
		
		if (*ptr == last) runlength ++;
		else {
			printf("%u\tx '%c'\n", runlength, last);
			runlength = 1;
			last = *ptr;
		}

		ptr ++;
	}

	printf("%u\tx '%c'\n", runlength, *(ptr-1));

	return 0;
}
