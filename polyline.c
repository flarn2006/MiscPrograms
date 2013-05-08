#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	int i;
	srand((unsigned)time(NULL));
	printf("%%PS-Adobe-3.0\n");
	for (i=0; i<50; i++)
	{
		float x, y;
		x = (float)rand() / RAND_MAX;
		y = (float)rand() / RAND_MAX;
		x *= 500;
		y *= 500;
		if (i == 0) printf("%.2f %.2f moveto\n", x, y);
		else printf("%.2f %.2f lineto\n", x, y);
	}
	printf("stroke showpage\n");
	return 0;
}
