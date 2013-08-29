#include <stdio.h>
#include <math.h>

void printAmount(long value)
{
	long a = value / 100L;
	int b = (int)value % 100;
	printf("$%ld.%02d", a, b);
}

void printUnit(long qty, const char *desc, long value)
{
	if (qty > 0) {
		printf("%ld %s\t= ", qty, desc);
		printAmount(qty * value);
		printf("\n");
	}
}

long numberOfUnits(long *curAmt, long value)
{
	long count = 0L;
	while (*curAmt >= value) {
		*curAmt -= value;
		count++;
	}
	return count;
}

int main(int argc, char *argv[])
{
	long bills100 = 0L, bills50 = 0L, bills20 = 0L;
	long bills10 = 0L, bills5 = 0L, bills1 = 0L;
	long quarters = 0L, dimes = 0L, nickels = 0L, pennies = 0L;
	
	long original, amount;
	double amount_f;

	if (argc >= 2) {
		const char *str = argv[1];
		if (*str == '$') str++;
		sscanf(str, "%lf", &amount_f);
	} else {
		printf("Enter an amount of money: $");
		scanf("%lf", &amount_f);
	}

	amount = lround(amount_f * 100.0);

	if (amount < 0L) {
		printf("Invalid amount!\n");
		return 1;
	}
	original = amount;

	bills100 = numberOfUnits(&amount, 10000L);
	bills50  = numberOfUnits(&amount,  5000L);
	bills20  = numberOfUnits(&amount,  2000L);
	bills10  = numberOfUnits(&amount,  1000L);
	bills5   = numberOfUnits(&amount,   500L);
	bills1   = numberOfUnits(&amount,   100L);
	quarters = numberOfUnits(&amount,    25L);
	dimes    = numberOfUnits(&amount,    10L);
	nickels  = numberOfUnits(&amount,     5L);
	pennies  = (int)amount;

	printf("\n");
	printAmount(original);
	printf(" is equal to:\n\n");

	printUnit(bills100, "$100 bills", 10000L);
	printUnit(bills50,  "$50 bills ", 5000L);
	printUnit(bills20,  "$20 bills ", 2000L);
	printUnit(bills10,  "$10 bills ", 1000L);
	printUnit(bills5,   "$5 bills  ", 500L);
	printUnit(bills1,   "$1 bills  ", 100L);
	printUnit(quarters, "quarters  ", 25L);
	printUnit(dimes,    "dimes     ", 10L);
	printUnit(nickels,  "nickels   ", 5L);
	printUnit(pennies,  "pennies   ", 1L);
	
	printf("\n");

	return 0;
}
