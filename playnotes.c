/* playnotes.c - Plays a string of notes given on the command line. */
/* Created by flarn2006 - http://flarn2006.blogspot.com/ */
/* Licensed under Creative Commons Attribution-ShareAlike 3.0 Unported */
/* Requires libao (http://xiph.org/ao/) - Compile with "-lao -lm" */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
#define SAMPLE_RATE 44100
#define NOTE_LENGTH 0.250
#define CLOCK_RATE 1190000

double tofreq(char note);

void play_note(int console_fd, double freq, double duration)
{
	unsigned long msec = (unsigned long)(1000.0 * duration);
	unsigned long arg = (unsigned long)(CLOCK_RATE / freq) | (msec << 16);
	ioctl(console_fd, KDMKTONE, arg);
	usleep(1000 * msec);
}

int main(int argc, char *argv[])
{
	double duration = NOTE_LENGTH;
	float octave_mult = 1.0f;
	int console_fd = open("/dev/console", O_RDWR);
	if (console_fd == -1) {
		perror("/dev/console");
		return 1;
	}

	if (argc == 2);  /* do nothing */
	else if (argc == 3)
	{
		sscanf(argv[2], "%lf", &duration);
		if (duration < 0)
		{
			printf("Note length can't be negative!\n");
			return 1;
		}
	}
	else
	{
		printf("Usage: %s notes [length]\n", argv[0]);
		return 3;
	}
	
	size_t length = strlen(argv[1]);
	int i; for (i=0; i<length; i++)
	{
		char ch = argv[1][i];
		char capital;
		switch (ch)
		{
			case '-': octave_mult *= 0.5f; break;
			case '+': octave_mult *= 2.0f; break;
			default:
				capital = ('A' <= ch && ch <= 'G');
				if (capital) octave_mult *= 2.0f;
				play_note(console_fd, tofreq(ch) * octave_mult, duration);
				if (capital) octave_mult *= 0.5f;
		}
	}
	
	return 0;
}

double tofreq(char note)
{
	switch (note)
	{
		case 'C': case 'c': return 261.63; break;
		case 'D': case 'd': return 293.66; break;
		case 'E': case 'e': return 329.63; break;
		case 'F': case 'f': return 349.23; break;
		case 'G': case 'g': return 392.00; break;
		case 'A': case 'a': return 440.00; break;
		case 'B': case 'b': return 493.88; break;
		default:  return 0.00;   break; /*rest*/
	}
}
