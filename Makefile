CC="gcc"
CPP="g++"
CFLAGS=-O3
TARGETS=addletters bestguess bounce chaos charsnow consine countpi crop exclude fecho graph hdraw hex life money movers myatoi particles pascaltri polyline rainbow roman rule90 ruler runlength safegets series spellnum stats throttle
TARGETS_LIBAO=playnotes

all: $(TARGETS) $(TARGETS_LIBAO)
nolibao: $(TARGETS)
clean:
	rm -f $(TARGETS) $(TARGETS_LIBAO)

addletters: addletters.c
	$(CC) $(CFLAGS) addletters.c -o addletters
bestguess: bestguess.cpp
	$(CPP) $(CFLAGS) bestguess.cpp -o bestguess
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
crop: crop.c
	$(CC) $(CFLAGS) crop.c -o crop
exclude: exclude.c
	$(CC) $(CFLAGS) exclude.c -o exclude
fecho: fecho.c
	$(CC) $(CFLAGS) fecho.c -o fecho
graph: graph.c
	$(CC) $(CFLAGS) graph.c -lform -lncurses -lmatheval -lm -o graph
hdraw: hdraw.c
	$(CC) $(CFLAGS) hdraw.c -o hdraw
hex: hex.c
	$(CC) $(CFLAGS) hex.c -o hex
life: life.c
	$(CC) $(CFLAGS) life.c -lncurses -o life
money: money.c spellnum.h spellnum.c
	$(CC) $(CFLAGS) money.c spellnum.c -lm -o money
movers: movers.c delayms.h
	$(CC) $(CFLAGS) movers.c -lncurses -o movers
myatoi: myatoi.c
	$(CC) $(CFLAGS) myatoi.c -o myatoi
particles: particles.cpp delayms.h
	$(CPP) $(CFLAGS) particles.cpp -lncurses -o particles
pascaltri: pascaltri.c
	$(CC) $(CFLAGS) pascaltri.c -o pascaltri
playnotes: playnotes.c
	$(CC) $(CFLAGS) playnotes.c -lao -lm -o playnotes
polyline: polyline.c
	$(CC) $(CFLAGS) polyline.c -o polyline
rainbow: rainbow.c
	$(CC) $(CFLAGS) rainbow.c -o rainbow
roman: roman.c
	$(CC) $(CFLAGS) roman.c -o roman
rule90: rule90.c
	$(CC) $(CFLAGS) rule90.c -o rule90
ruler: ruler.c
	$(CC) $(CFLAGS) ruler.c -o ruler
runlength: runlength.c
	$(CC) $(CFLAGS) runlength.c -o runlength
safegets: safegets.c
	$(CC) $(CFLAGS) safegets.c -o safegets
series: series.c
	$(CC) $(CFLAGS) series.c -o series
spellnum: spellnum.c spellnum.h
	$(CC) $(CFLAGS) -DSPELLNUM_STANDALONE spellnum.c -o spellnum
stats: stats.c
	$(CC) $(CFLAGS) stats.c -lm -o stats
throttle: throttle.c delayms.h
	$(CC) $(CFLAGS) throttle.c -o throttle
unhdraw: unhdraw.c
	$(CC) $(CFLAGS) unhdraw.c -o unhdraw
