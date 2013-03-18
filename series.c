#include <stdio.h>
#include <stdlib.h>

/* Sample:
Enter voltage: 36
How many resistors? 3
R1 = 6
R2 = 5
R3 = 12

        V       I       R
R1      9.391   1.565   6.000
R2      7.826   1.565   5.000
R3      18.783  1.565   12.000 */

double array_sum(double* array, int count);

int main(int argc, char *argv[])
{
    int count, n;
    double* v;
    double* r;
    double vt, it, rt;
    
    printf("Enter voltage: ");
    scanf("%lf", &vt);
    printf("How many resistors? ");
    scanf("%d", &count);
    
    v = malloc(sizeof(double) * count);
    r = malloc(sizeof(double) * count);
    
    for (n=0; n<count; n++)
    {
        printf("R%d = ", n+1);
        scanf("%lf", r+n);
    }
    
    rt = array_sum(r, count);
    it = vt / rt;
    
    for (n=0; n<count; n++) v[n] = it * r[n];
    
    printf("\n\tV\tI\tR\n");
    for (n=0; n<count; n++)
    {
        printf("R%d\t%.3lf\t%.3lf\t%.3lf\n", n+1, v[n], it, r[n]);
    }
    
    free(v); free(r);
    return 0;
}

double array_sum(double* array, int count)
{
    double sum = 0;
    int n;
    for (n=0; n<count; n++) sum += array[n];
    return sum;
}
