#include "common.h"
#include "RPSGame.h"
#include "RPSPlayer.h"
#include "Players.h"
#include "RPSTournamentMatchFormat.h"
#include "RPSTournamentMatchFormats.h"
#include "RPSTournament.h"

int main(int argc, char **argv) {
  srand(time(nullptr));
  RPSTournamentMatchFormat *format = RPSTournamentMatchFormatFactory::createRPSTournamentMatchFormat("Comp322RPSTournamentMatchFormat");
  // RPSTournamentMatchFormat *format = RPSTournamentMatchFormatFactory::createRPSTournamentMatchFormat("BestOfThreeRPSTournamentMatchFormat");
  RPSTournament oRPSTournament(128, format);
  oRPSTournament.play();
  RPSTournamentMatchFormatFactory::recycle(format);
}