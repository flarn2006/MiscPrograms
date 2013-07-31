CC="/usr/bin/gcc"
CFLAGS=-O3 -Wall
TARGETS=addletters consine expand graph life money playnotes polyline rainbow rule90 runlength series

all: $(TARGETS)
clean:
	rm $(TARGETS)

addletters: addletters.c
	$(CC) $(CFLAGS) addletters.c -o addletters
consine: consine.c
	$(CC) $(CFLAGS) consine.c -lncurses -lm -o consine
expand: expand.c
	$(CC) $(CFLAGS) expand.c -o expand
graph: graph.c
	$(CC) $(CFLAGS) graph.c -lncurses -lmatheval -lm -o graph
life: life.c
	$(CC) $(CFLAGS) life.c -lncurses -o life
money: money.c
	$(CC) $(CFLAGS) money.c -o money
playnotes: playnotes.c
	$(CC) $(CFLAGS) playnotes.c -lao -lm -o playnotes
polyline: polyline.c
	$(CC) $(CFLAGS) polyline.c -o polyline
rainbow: rainbow.c
	$(CC) $(CFLAGS) rainbow.c -o rainbow
rule90: rule90.c
	$(CC) $(CFLAGS) rule90.c -o rule90
runlength: runlength.c
	$(CC) $(CFLAGS) runlength.c -o runlength
series: series.c
	$(CC) $(CFLAGS) series.c -o series
