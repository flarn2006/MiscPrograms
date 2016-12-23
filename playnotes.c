/* playnotes.c - Plays a string of notes given on the command line. */
/* Created by flarn2006 - http://flarn2006.blogspot.com/ */
/* Licensed under Creative Commons Attribution-ShareAlike 3.0 Unported */
/* Requires libao (http://xiph.org/ao/) - Compile with "-lao -lm" */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <ao/ao.h>
#define SAMPLE_RATE 44100
#define NOTE_LENGTH 0.250

double tofreq(char note);

void play_note(ao_device *device, double freq, double duration)
{
	unsigned long i;
	size_t length = SAMPLE_RATE * duration;
	int16_t *audio = malloc(length*2);
	
	for (i=0; i<length; i++)
	{
		double value = sin(i * 2 * M_PI * (freq / (double)SAMPLE_RATE));
		audio[i] = (int16_t)(value * 0x7FFF);
	}
	
	ao_play(device, (char*)audio, length*2);
	
	free(audio);
}

int main(int argc, char *argv[])
{
	ao_device *device;
	ao_sample_format fmt;
	double duration = NOTE_LENGTH;
	float octave_mult = 1.0f;

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
	
	fmt.bits = 16;
	fmt.rate = SAMPLE_RATE;
	fmt.channels = 1;
	fmt.byte_format = AO_FMT_NATIVE;
	fmt.matrix = NULL;
	
	ao_initialize();
	
	device = ao_open_live(ao_default_driver_id(), &fmt, NULL);
	if (!device) {
		printf("Error %d opening audio device!\n", errno);
		return 2;
	}
	
	int i; for (i=0; i<strlen(argv[1]); i++)
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
				play_note(device, tofreq(ch) * octave_mult, duration);
				if (capital) octave_mult *= 0.5f;
		}
	}
	
	ao_close(device);
	ao_shutdown();
	
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
