#include <stdio.h>

int numberOfUnits(double *curAmt, double value)
{
	int count = 0;
	while (*curAmt >= value) {
		*curAmt -= value;
		count++;
	}
	return count;
}

int main(int argc, char *argv[])
{
	int bills100 = 0, bills50 = 0, bills20 = 0;
	int bills10 = 0, bills5 = 0, bills1 = 0;
	int quarters = 0, dimes = 0, nickels = 0, pennies = 0;
	
	double original, amount;

	if (argc >= 2) {
		const char *str = argv[1];
		if (*str == '$') str++;
		sscanf(str, "%lf", &amount);
	} else {
		printf("Enter an amount of money: $");
		scanf("%lf", &amount);
	}

	if (amount < 0.01) {
		printf("Invalid amount!\n");
		return 1;
	}
	original = amount;

	bills100 = numberOfUnits(&amount, 100.00);
	bills50  = numberOfUnits(&amount,  50.00);
	bills20  = numberOfUnits(&amount,  20.00);
	bills10  = numberOfUnits(&amount,  10.00);
	bills5   = numberOfUnits(&amount,   5.00);
	bills1   = numberOfUnits(&amount,   1.00);
	quarters = numberOfUnits(&amount,   0.25);
	dimes    = numberOfUnits(&amount,   0.10);
	nickels  = numberOfUnits(&amount,   0.05);
	pennies  = (int)(amount * 100.00);

	printf("\n");
	printf("$%.2lf is equal to:\n\n", original);

	if (bills100) printf("%d $100 bills\t= $%.2lf\n", bills100, bills100 * 100.00);
	if (bills50)  printf("%d $50 bills\t= $%.2lf\n", bills50, bills50 * 50.00);
	if (bills20)  printf("%d $20 bills\t= $%.2lf\n", bills20, bills20 * 20.00);
	if (bills10)  printf("%d $10 bills\t= $%.2lf\n", bills10, bills10 * 10.00);
	if (bills5)   printf("%d $5 bills\t= $%.2lf\n", bills5, bills5 * 5.00);
	if (bills1)   printf("%d $1 bills\t= $%.2lf\n", bills1, (double)bills1);
	if (quarters) printf("%d quarters\t= $%.2lf\n", quarters, quarters * 0.25);
	if (dimes)    printf("%d dimes\t\t= $%.2lf\n", dimes, dimes * 0.10);
	if (nickels)  printf("%d nickels\t= $%.2lf\n", nickels, nickels * 0.05);
	if (pennies)  printf("%d pennies\t= $%.2lf\n", pennies, pennies * 0.01);
	
	printf("\n");

	return 0;
}
