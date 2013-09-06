CC="/usr/bin/gcc"
CPP="/usr/bin/g++"
CFLAGS=-O3
TARGETS=addletters bounce chaos charsnow consine countpi graph life money movers particles polyline rainbow rule90 runlength series stats
TARGETS_LIBAO=playnotes

all: $(TARGETS) $(TARGETS_LIBAO)
nolibao: $(TARGETS)
clean:
	rm -f $(TARGETS) $(TARGETS_LIBAO)

addletters: addletters.c
	$(CC) $(CFLAGS) addletters.c -o addletters
bounce: bounce.c delayms.h
	$(CC) $(CFLAGS) bounce.c -lncurses -o bounce
chaos: chaos.c
	$(CC) $(CFLAGS) chaos.c -lncurses -lm -o chaos
charsnow: charsnow.c delayms.h
	$(CC) $(CFLAGS) charsnow.c -lncurses -o charsnow
consine: consine.c
	$(CC) $(CFLAGS) consine.c -lncurses -lm -o consine
countpi: countpi.c
	$(CC) $(CFLAGS) countpi.c -o countpi
graph: graph.c
	$(CC) $(CFLAGS) graph.c -lform -lncurses -lmatheval -lm -o graph
life: life.c
	$(CC) $(CFLAGS) life.c -lncurses -o life
money: money.c
	$(CC) $(CFLAGS) money.c -lm -o money
movers: movers.c delayms.h
	$(CC) $(CFLAGS) movers.c -lncurses -o movers
particles: particles.cpp delayms.h
	$(CPP) $(CFLAGS) particles.cpp -lncurses -o particles
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
stats: stats.c
	$(CC) $(CFLAGS) stats.c -lm -o stats
