#include "common.h"
#include "RPSGame.h"
#include "RPSPlayer.h"
#include "Players.h"
#include "RPSTournament.h"

int main(int argc, char **argv) {
  /* TODO different tournament or game formats */
  RPSTournament oRPSTournament(128);
  oRPSTournament.play();
}