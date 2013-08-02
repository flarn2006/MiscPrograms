#include <vector>
#include <cstdlib>
#include <ctime>
#include <curses.h>
#include "delayms.h"

#define TIME_STEP 0.01f
#define MAX_PARTICLES 256
#define VEL_INCREMENT 0.5f
#define ACCEL_INCREMENT 2.0f

class Particle
{
	private:
		float vx, vy;
		float ax, ay;
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

			ax = ay = 0.0f;
		}

		void simulate(float time)
		{
			x += vx * time;
			y += vy * time;

			vx += ax * time;
			vy += ay * time;

			age += time;
		}

		float getAge()
		{
			return age;
		}

		void setAccel(float accelX, float accelY)
		{
			ax = accelX;
			ay = accelY;
		}
};

int main(int argc, char *argv[])
{
	int xmax, ymax;
	float originX, originY;
	
	float velocity = 10.0f;
	float accelX = 0.0f;
	float accelY = 0.0f;
	int displayOn = 1;
	
	std::vector<Particle> *psys = new std::vector<Particle>();
	
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
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

	curs_set(0);

	int quitNow = 0;
	while (!quitNow) {
		erase();

		std::vector<Particle>::iterator i;
		for (i=psys->begin(); i!=psys->end(); i++) {
			i->simulate(TIME_STEP);
			float age = i->getAge();
			int pair;
			
			if (age < 0.75f) {
				pair = 1;
			} else if (age < 1.5f) {
				pair = 2;
			} else {
				pair = 3;
			}

			attron(COLOR_PAIR(pair));
			mvprintw((int)i->y, (int)i->x * 2, "()");
			attroff(COLOR_PAIR(pair));

			i->setAccel(accelX, accelY);
		}

		Particle p(originX, originY, velocity);
		psys->insert(psys->begin(), p);

		if (psys->size() > MAX_PARTICLES) {
			psys->pop_back();
		}

		if (displayOn) {
			attron(A_BOLD); attron(COLOR_PAIR(4));
			mvprintw((int)originY, (int)originX * 2, "<>");
			mvprintw(0, 0, "[-/+] Max Velocity:   %.1f", velocity);
			mvprintw(1, 0, "[A/D] X Acceleration: %.0f", accelX);
			mvprintw(2, 0, "[W/S] Y Acceleration: %.0f", accelY);
			attroff(A_BOLD); attroff(COLOR_PAIR(4));
		}

		refresh();
		delayms(1000 * TIME_STEP);

		switch (getch()) {
			case KEY_UP:    originY--; break;
			case KEY_DOWN:  originY++; break;
			case KEY_LEFT:  originX--; break;
			case KEY_RIGHT: originX++; break;

			case '+': case '=': velocity += VEL_INCREMENT; break;
			case '-': velocity -= VEL_INCREMENT; break;

			case 'a': accelX -= ACCEL_INCREMENT; break;
			case 'd': accelX += ACCEL_INCREMENT; break;
			case 'w': accelY -= ACCEL_INCREMENT; break;
			case 's': accelY += ACCEL_INCREMENT; break;
			
			case '`': displayOn = !displayOn; break;
			case 'q': quitNow = 1; break;
		}
	}

	endwin();
	delete psys;
	return 0;
}
