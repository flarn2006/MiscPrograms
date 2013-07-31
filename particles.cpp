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
	std::vector<Particle> *psys = new std::vector<Particle>();
	
	initscr();
	cbreak();
	noecho();

	start_color();
	init_pair(1, 2, 0); //green
	init_pair(2, 3, 0); //yellow
	init_pair(3, 1, 0); //red

	while (1) {
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

		Particle p(20.0f, 12.5f, 10.0f);
		psys->insert(psys->begin(), p);

		if (psys->size() > MAX_PARTICLES) {
			psys->pop_back();
		}

		refresh();
		delayms(1000 * TIME_STEP);
	}

	endwin();
	return 0;
}
