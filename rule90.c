#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM_CELLS 71
#define NUM_GENERATIONS 32
#define char_ON  '#'
#define char_OFF ' '

void do_generation(char *cells, int num_cells)
{
	int i;
	char *new_cells = (char*)malloc(num_cells + 1);
	new_cells[0] = 0 ^ cells[1];
	new_cells[num_cells-1] = cells[num_cells-2] ^ 0;
	
	for (i=1; i<=num_cells-2; i++)
	{
		new_cells[i] = cells[i-1] ^ cells[i+1];
	}
	
	memcpy(cells, new_cells, num_cells-1);
	free(new_cells);
}
	
int main()
{
	char cells[NUM_CELLS];
	int i, j;
	
	for (i=0; i<NUM_CELLS; i++) cells[i] = (char)0;
	cells[(NUM_CELLS-1)/2] = (char)1;
	
	for (i=1; i<=NUM_GENERATIONS; i++)
	{
		for (j=0; j<NUM_CELLS; j++)
		{
			printf("%c", cells[j] ? char_ON:char_OFF);
		}
		printf("\n");
		do_generation(cells, NUM_CELLS);
	}
	
	return 0;
}