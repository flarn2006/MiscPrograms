#include <vector>
#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include "delayms.h"

#define TIME_STEP 0.01f
#define MAX_PARTICLES 256

class Particle
{
	private:
		float vx, vy;
		float age;

	public:
		float x, y;
		
		Particle(float startX, float startY, float maxSpeed)
		{
			x = startX;
			y = startY;
			
			vx = (float)rand() / (0.5f*RAND_MAX) - 1.0f;
			vy = (float)rand() / (0.5f*RAND_MAX) - 1.0f;
			vx *= maxSpeed; vy *= maxSpeed;
		}

		void simulate(float time)
		{
			x += vx * time;
			y += vy * time;
			age += time;
		}

		float getAge()
		{
			return age;
		}
};

int main(int argc, char *argv[])
{
	int xmax, ymax;
	float originX, originY;
	std::vector<Particle> *psys = new std::vector<Particle>();
	
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	nodelay(stdscr, TRUE);
	getmaxyx(stdscr, ymax, xmax);

	originX = (float)(xmax+1) / 4.0f;
	originY = (float)(ymax+1) / 2.0f;

	start_color();
	init_pair(1, 2, 0); //green
	init_pair(2, 3, 0); //yellow
	init_pair(3, 1, 0); //red
	init_pair(4, 6, 0); //cyan

	erase();
	printw("Use the arrow keys to move, and press 'q' to stop...");
	refresh();
	delayms(1000);

	int quitNow = 0;
	
	while (!quitNow) {
		erase();

		std::vector<Particle>::iterator i;
		for (i=psys->begin(); i!=psys->end(); i++) {
			(*i).simulate(TIME_STEP);
			float age = (*i).getAge();
			int pair;
			
			if (age < 0.75f) {
				pair = 1;
			} else if (age < 1.5f) {
				pair = 2;
			} else {
				pair = 3;
			}

			attron(COLOR_PAIR(pair));
			mvprintw((int)(*i).y, (int)(*i).x * 2, "()");
			attroff(COLOR_PAIR(pair));
		}

		Particle p(originX, originY, 10.0f);
		psys->insert(psys->begin(), p);

		if (psys->size() > MAX_PARTICLES) {
			psys->pop_back();
		}

		attron(A_BOLD); attron(COLOR_PAIR(4));
		mvprintw((int)originY, (int)originX * 2, "<>");
		attroff(A_BOLD); attroff(COLOR_PAIR(4));

		refresh();
		delayms(1000 * TIME_STEP);

		switch (getch()) {
			case KEY_UP:    originY--; break;
			case KEY_DOWN:  originY++; break;
			case KEY_LEFT:  originX--; break;
			case KEY_RIGHT: originX++; break;
			
			case 'q': quitNow = 1; break;
		}
	}

	endwin();
	return 0;
}
