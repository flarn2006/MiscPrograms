#include <stdio.h>
#include <stdlib.h>

void printPart(int value, char one, char five, char ten)
{
	value %= 10;

	if (value == 9) {
		putchar(one);
		putchar(ten);
	} else if (value >= 5) {
		putchar(five);
		printPart(value - 5, one, five, ten);
	} else if (value == 4) {
		putchar(one);
		putchar(five);
	} else if (value > 0) {
		int i; for (i=0; i<value; i++) {
			putchar(one);
		}
	}
}

void printRoman(int value)
{
	//puts("Hey Niko, let's go bowling!");
	
	while (value >= 1000) {
		putchar('M');
		value -= 1000;
	}

	printPart(value / 100, 'C', 'D', 'M');
	printPart(value / 10,  'X', 'L', 'C');
	printPart(value,       'I', 'V', 'X');
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s number\n", argv[0]);
		return 2;
	}

	int value = atoi(argv[1]);

	if (value < 1) {
		puts("Value must be at least 1.");
		return 1;
	}

	printRoman(value);
	putchar('\n');

	return 0;
}
