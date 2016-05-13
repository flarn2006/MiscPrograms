#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SQ(x) ((x)*(x))
#define INPUT_BUF_SIZE 256

struct stats {
    int n;          /* count */
    double sum;     /* sum of all values */
    double avg;     /* mean of all values */
    double sqsum;   /* sum of squares of differences from mean */
    double stddev;  /* standard deviation */
};

void addValue(struct stats *st, double x)
{
    st->n ++;                                      /* increment the count */
    st->sum += x;                                  /* update the sum */
    st->avg = st->sum / (double)st->n;             /* update the average */
    st->sqsum += SQ(st->avg-x);                    /* update the SoSoDfM */
    st->stddev = sqrt(st->sqsum / (double)st->n);  /* update the standard deviation */
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
    
    /* Initialize the structure */
    initStats(&st);
    
    printf("Type 'r' to reset or 'q' to quit.\n\n");

    while (!quitNow) {
        /* Display the prompt */
        printf("%3d> ", st.n);

        /* Get the input from the user */
        fgets(input, INPUT_BUF_SIZE, stdin);
        
        /* Check for the quit/reset command; quit or add value */
        if (input[0] == 'q' || input[0] == 'Q') {
            /* Quit was requested */
            quitNow = 1;
        } else if (input[0] == 'r' || input[0] == 'R') {
            /* Reset was requested */
            initStats(&st);
        } else if (input[0] == '\0' || input[0] == '\n') {
            /* Nothing was given */
        } else {
            /* A value was given */
            addValue(&st, atof(input));
            printf("\n");
            printf("sum =    %lg\n", st.sum);
            printf("avg =    %lg\n", st.avg);
            printf("stddev = %lg\n", st.stddev);
            printf("\n");
        }
    }

    return 0;
}
