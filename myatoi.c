#include <stdio.h>
#include <string.h>

int my_atoi(const char *str)
{
	/* Check for a minus sign and skip past it. */
	int negative = 0;
	if (*str == '-') {
		negative = 1;
		str++;
	}

	/* Find the first non-numeric character. */
	int index = 0;
	while ('0' <= *str && *str <= '9') {
		index++; str++;
	}

	/* If index is still 0, then there's no number. */
	if (index == 0) return 0;

	/* Now actually determine the value. */
	int pow10 = 1;
	int value = 0;
	index--; str--;
	while (index >= 0) {
		int digit = *str - '0'; /*get value of digit*/
		value += digit * pow10;
		pow10 *= 10;
		index--; str--;
	}

	/* Return the value, negated if necessary. */
	if (negative) value *= -1;
	return value;
}

int main(int argc, char *argv[])
{
	printf("Please enter a number: ");
	
	char buf[12]; /*enough room for "-2147483648\0"*/
	fgets(buf, 12, stdin);
	char *newline = strchr(buf, '\n');
	if (newline) *newline = '\0';

	printf("I got \"%s\".\n", buf);
	printf("I interpreted that as %d.\n", my_atoi(buf));

	return 0;
}
