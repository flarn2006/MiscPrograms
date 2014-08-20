// IMPORTANT! If you don't have delayms.h, get it from here:
// https://github.com/flarn2006/MiscPrograms/blob/master/delayms.h

#include <vector>
#include <cstdlib>
#include <ctime>
#include <curses.h>
#include "delayms.h"

#define TIME_STEP 0.01f
#define MAX_PARTICLES 4096
#define VEL_INCREMENT 0.5f
#define ACCEL_INCREMENT 2.0f
#define AGE_INCREMENT 0.2f

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

			age = 0.0f;
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
	float maxAge = 1.0f;
	char display = 3;
	
	std::vector<Particle> *psys = new std::vector<Particle>();
	
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(0);
	getmaxyx(stdscr, ymax, xmax);

	originX = (float)(xmax+1) / 4.0f;
	originY = (float)(ymax+1) / 2.0f;

	start_color();
	init_pair(1, 2, 0); //green
	init_pair(2, 3, 0); //yellow
	init_pair(3, 1, 0); //red
	init_pair(4, 6, 0); //cyan

	int quitNow = 0;
	while (!quitNow) {
		erase();

		std::vector<Particle>::iterator i;
		for (i=psys->begin(); i!=psys->end(); i++) {
			i->simulate(TIME_STEP);
			float age = i->getAge();
			
			if (age <= maxAge) {
				int pair;
				if (age < maxAge * 0.33f) {
					pair = 1;
				} else if (age < maxAge * 0.66f) {
					pair = 2;
				} else {
					pair = 3;
				}

				attron(A_BOLD); attron(COLOR_PAIR(pair));
				mvprintw((int)i->y, (int)i->x * 2, "()");
				attroff(COLOR_PAIR(pair)); attroff(A_BOLD);
			}

			i->setAccel(accelX, accelY);
		}

		Particle p(originX, originY, velocity);
		psys->push_back(p);

		if (psys->size() > MAX_PARTICLES) {
			psys->erase(psys->begin());
		}

		attron(A_BOLD); attron(COLOR_PAIR(4));
		if (display & 1) {
			mvprintw((int)originY, (int)originX * 2, "<>");
		}
		if (display & 2) {
			mvprintw(0, 0, "<Arrows> Move emitter");
			mvprintw(1, 0, "<-/+> Max Velocity:   %.1f", velocity);
			mvprintw(2, 0, "<A/D> X Acceleration: %.0f", accelX);
			mvprintw(3, 0, "<W/S> Y Acceleration: %.0f", accelY);
			mvprintw(4, 0, "<[/]> Maximum age:    %.1f", maxAge);
			mvprintw(5, 0, "<X>   Clear all particles");
			mvprintw(6, 0, "<`>   Change display");
			mvprintw(7, 0, "<Q>   Quit program");
		}
		attroff(A_BOLD); attroff(COLOR_PAIR(4));

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

			case '[': maxAge -= AGE_INCREMENT; if (maxAge < 0) maxAge = 0; break;
			case ']': maxAge += AGE_INCREMENT; break;
			
			case 'x': psys->clear(); break;
			case '`': display ++; break;
			case 'q': quitNow = 1; break;
		}
	}

	endwin();
	delete psys;
	return 0;
}
