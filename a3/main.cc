#include "common.h"
#include "RPSGame.h"
#include "RPSPlayer.h"
#include "Players.h"
#include "RPSTournament.h"

int main(int argc, char **argv) {
  RPSTournament oRPSTournament(128);
  oRPSTournament.play();
}