#include <stdio.h>
#define SQ(x) ((x)*(x))

// Increase diameter to increase accuracy at cost of speed.
#define DIAMETER 10000

int inCircle(int x, int y, int d)
{
	// (x-h)^2 + (y-k)^2 < r^2
	// in this case, h and k both equal r
	double r = (double)d / 2.0;
	return SQ(x-r) + SQ(y-r) < SQ(r);
}

inline double estimatePi(double A, double d)
{
	// A = area; d = diameter
	// pi = 4A / d^2
	return 4.0*A / SQ(d);
}

int main(int argc, char *argv[])
{
	int count = 0;
	int x, y;
	
	for (y=0; y<DIAMETER; y++)
	for (x=0; x<DIAMETER; x++) {
		if (inCircle(x, y, DIAMETER)) {
			count ++;
		}
	}

	double pi = estimatePi((double)count, (double)DIAMETER);
	printf("Pi is approximately %.5lf.\n", pi);

	return 0;
}
