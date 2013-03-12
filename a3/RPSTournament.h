#ifndef RPSTOURNAMENT_H
#define RPSTOURNAMENT_H

#include "common.h"
#include "RPSPlayer.h"

class RPSTournament {
public:
  typedef std::vector<RPSPlayer*> roster_type;
  RPSTournament(unsigned int numPlayers);
  ~RPSTournament();
  void play();
private:
  void initPlayers(unsigned int numPlayers);
  void advance(RPSPlayer *winner);
  void eliminate(RPSPlayer *loser);
  roster_type roster;
};

#endif