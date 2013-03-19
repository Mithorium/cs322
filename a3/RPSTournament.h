#ifndef RPSTOURNAMENT_H
#define RPSTOURNAMENT_H

#include "common.h"
#include "RPSPlayer.h"
#include "RPSTournamentMatchFormat.h"

class RPSTournament {
public:
  typedef std::vector<RPSPlayer*> roster_type;
  RPSTournament(unsigned int numPlayers, RPSTournamentMatchFormat *format);
  ~RPSTournament();
  void play();
private:
  void initPlayers(unsigned int numPlayers);
  void advance(RPSPlayer *winner);
  void eliminate(RPSPlayer *loser);
  roster_type roster;
  RPSTournamentMatchFormat *matchFormat;
};

#endif