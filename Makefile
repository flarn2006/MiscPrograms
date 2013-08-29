CC="/usr/bin/gcc"
CPP="/usr/bin/g++"
CFLAGS=-O3
TARGETS=addletters bounce charsnow consine countpi expand graph life money particles polyline rainbow rule90 runlength series stats
TARGETS_LIBAO=playnotes

all: $(TARGETS) $(TARGETS_LIBAO)
nolibao: $(TARGETS)
clean:
	rm -f $(TARGETS) $(TARGETS_LIBAO)

addletters: addletters.c
	$(CC) $(CFLAGS) addletters.c -o addletters
bounce: bounce.c delayms.h
	$(CC) $(CFLAGS) bounce.c -lncurses -o bounce
charsnow: charsnow.c delayms.h
	$(CC) $(CFLAGS) charsnow.c -lncurses -o charsnow
consine: consine.c
	$(CC) $(CFLAGS) consine.c -lncurses -lm -o consine
countpi: countpi.c
	$(CC) $(CFLAGS) countpi.c -o countpi
expand: expand.c
	$(CC) $(CFLAGS) expand.c -o expand
graph: graph.c
	$(CC) $(CFLAGS) graph.c -lncurses -lmatheval -lm -o graph
life: life.c
	$(CC) $(CFLAGS) life.c -lncurses -o life
money: money.c
	$(CC) $(CFLAGS) money.c -o money
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
	$(CC) $(CFLAGS) stats.c -o stats
