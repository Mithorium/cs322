# Second one for gdb debugging
CC = g++ -O2 -Wall -std=c++0x
# CC = g++ -g -O0 -Wall -std=c++0x

all: main

main: common.h RPSPlayer.h Players.h Players.cc RPSGame.h RPSGame.cc RPSTournamentMatchFormat.h RPSTournamentMatchFormats.h RPSTournamentMatchFormats.cc RPSTournament.h RPSTournament.cc main.cc
	$(CC) Players.cc RPSGame.cc RPSTournamentMatchFormats.cc RPSTournament.cc main.cc -o RPSTournament

clean:
	rm -f RPSTournament
