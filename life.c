#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BV(x,y) ((field[x][y] == '#' || field[x][y] == 'X') ? 1:0);
#define WIDTH 40
#define HEIGHT 23

char field[WIDTH][HEIGHT];

int neighbors(int x, int y)
{
	int count = 0;
	if (x > 0 && y > 0)					count += BV(x-1,y-1);
	if (y > 0)							count += BV(x,y-1);
	if (x < WIDTH-1 && y > 0)			count += BV(x+1,y-1);
	if (x > 0)							count += BV(x-1,y);
	if (x < WIDTH-1)					count += BV(x+1,y);
	if (x > 0 && y < HEIGHT-1)			count += BV(x-1,y+1);
	if (y < HEIGHT-1)					count += BV(x,y+1);
	if (x < WIDTH-1 && y < HEIGHT-1)	count += BV(x+1,y+1);
	return count;
}

int loadWorld(const char *filename, char field[][HEIGHT], const char *argv0)
{
	int y;
	FILE *fp = strcmp(filename, "-") ? fopen(filename, "r") : stdin;
	if (!fp)
	{
		perror(argv0);
		return 2;
	}

	memset(field, ' ', WIDTH*HEIGHT);
	
	for (y=0; y<HEIGHT; y++)
	{
		int x, length;
		char line[WIDTH+1];

		fgets(line, WIDTH, fp);
		if (feof(fp)) break;
		length = strlen(line);
		
		for (x=0; x<length; x++)
		{
			char ch = line[x];
			if (ch == ' ' || ch == '#') field[x][y] = ch;
			else if (ch == '\n') break;
			else
			{
				fprintf(stderr, "Unknown character '%c' found.\n", ch);
				fprintf(stderr, "Acceptable characters are '#' and ' '.\n");
				return 1;
			}
		}
	}
	
	if (fp != stdin) fclose(fp);
	
	return 0;
}
				

int main(int argc, const char *argv[])
{
	int x, y;
	int gen = 0;
	int result = 0;
	
	switch (argc)
	{
		case 1:
			srand((unsigned)time(NULL));
			for (y=0; y<HEIGHT; y++)
			for (x=0; x<WIDTH; x++)
			{
				field[x][y] = rand() % 2 ? '#':' ';
			}
			break;
			
		case 2:
			result = loadWorld(argv[1], field, argv[0]);
			if (result) return result;
			break;
			
		default:
			printf("Usage: %s [filename | -]\n", argv[0]);
			return 3;
			break;
	}
	
	initscr(); cbreak(); noecho();
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
			
	for (;;)
	{
		erase();
		attron(COLOR_PAIR(1) | A_BOLD);
		mvprintw(23, 0, "Generation: %d", gen);
		attroff(COLOR_PAIR(1) | A_BOLD);
		for (y=0; y<HEIGHT; y++)
			for (x=0; x<WIDTH; x++)
			{
				if (field[x][y] == '#') mvprintw(y, x*2, "[]");
			}
		refresh();
		if (getch() == 'q') break;
		
		gen ++;
		
		for (y=0; y<HEIGHT; y++)
			for (x=0; x<WIDTH; x++)
			{
				if (neighbors(x,y) == 3 && field[x][y] == ' ')
					field[x][y] = '+';
				if (!(neighbors(x,y) == 2 || neighbors(x,y) == 3) && field[x][y] == '#')
					field[x][y] = 'X';
			}
		
		for (y=0; y<HEIGHT; y++)
			for (x=0; x<WIDTH; x++)
			{
				if (field[x][y] == 'X') field[x][y] = ' ';
				if (field[x][y] == '+') field[x][y] = '#';
			}
	}
	
	endwin();
	return 0;
}

