#include "RPSTournamentMatchFormats.h"

void Comp322RPSTournamentMatchFormat::playMatch(RPSGame &oRPSGame, RPSGame::Result &result) {
  unsigned char rounds = 3;
  do {
    result = oRPSGame.playRound();
  } while (--rounds && result == RPSGame::TIE);
}
REGISTER_RPSMATCHFORMAT(Comp322RPSTournamentMatchFormat);

void BestOfThreeRPSTournamentMatchFormat::playMatch(RPSGame &oRPSGame, RPSGame::Result &result) {
  result = oRPSGame.play(3);
}
REGISTER_RPSMATCHFORMAT(BestOfThreeRPSTournamentMatchFormat);