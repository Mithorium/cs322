#ifndef RPSTOURNAMENTMATCHFORMATS_H
#define RPSTOURNAMENTMATCHFORMATS_H

#include "RPSTournamentMatchFormat.h"

class Comp322RPSTournamentMatchFormat : public RPSTournamentMatchFormat {
public:
  void playMatch(RPSGame &oRPSGame, RPSGame::Result &result);
protected:
  const char* description() const { return "Best of 1 w/ 2 tiebreakers"; }
private:
  DECLARE_RPSMATCHFORMAT(Comp322RPSTournamentMatchFormat);
};

class BestOfThreeRPSTournamentMatchFormat : public RPSTournamentMatchFormat {
public:
  void playMatch(RPSGame &oRPSGame, RPSGame::Result &result);
protected:
  const char* description() const { return "Best of 3"; }
private:
  DECLARE_RPSMATCHFORMAT(BestOfThreeRPSTournamentMatchFormat);
};

#endif