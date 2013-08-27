#include <stdio.h>
#include <stdlib.h>

#define INPUT_BUF_SIZE 256

struct stats {
	int n;			// count
	double sum;		// sum of all values
	double avg;		// mean of all values
};

void addValue(struct stats *st, double x)
{
	st->n ++;							// increment the count
	st->sum += x;						// update the sum
	st->avg = st->sum / (double)st->n;	// update the average
}

void initStats(struct stats *st)
{
	st->n = 0;
	st->sum = 0.0;
}

int main(int argc, char *argv[])
{
	int quitNow = 0;
	struct stats st;
	char input[INPUT_BUF_SIZE];
	
	// Initialize the structure
	initStats(&st);
	
	printf("Type 'r' to reset or 'q' to quit.\n\n");

	while (!quitNow) {
		// Display the prompt
		printf("%3d> ", st.n);

		// Get the input from the user
		fgets(input, INPUT_BUF_SIZE, stdin);
		
		// Check for the quit/reset command; quit or add value
		if (input[0] == 'q' || input[0] == 'Q') {
			// Quit was requested
			quitNow = 1;
		} else if (input[0] == 'r' || input[0] == 'R') {
			// Reset was requested
			initStats(&st);
		} else if (input[0] == '\0' || input[0] == '\n') {
			// Nothing was given
		} else {
			// A value was given
			addValue(&st, atof(input));
			printf("\n");
			printf("sum =\t%lg\n", st.sum);
			printf("avg =\t%lg\n", st.avg);
			printf("\n");
		}
	}

	return 0;
}
