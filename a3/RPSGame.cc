#include "RPSGame.h"
#include "RPSPlayer.h"

RPSGame::RPSGame(RPSPlayer *playerOne, RPSPlayer *playerTwo) : p1(playerOne), p2(playerTwo) {
  p1->onNewOpponent();
  p2->onNewOpponent();
}

RPSGame::Result RPSGame::play(unsigned short int rounds) {
  unsigned short int p1wins = 0, p2wins = 0;
  for(;rounds;--rounds) {
    switch (playRound()) {
      case PLAYERONEWIN:
        ++p1wins;
        break;
      case PLAYERTWOWIN:
        ++p2wins;
        break;
      default:
        break;
    }
  }
  if (p1wins > p2wins) {
    return PLAYERONEWIN;
  } else if (p2wins > p1wins) {
    return PLAYERTWOWIN;
  } else {
    return TIE;
  }
}

RPSGame::Result RPSGame::playRound() {
  Move p1Move = p1->chooseMove();
  Move p2Move = p2->chooseMove();
  p1->onRound(p1Move,p2Move);
  p2->onRound(p2Move,p1Move);
  return evaluateMove(p1Move,p2Move);
}

RPSGame::Result RPSGame::evaluateMove(RPSGame::Move p1Move, RPSGame::Move p2Move) {
  if (p1Move == p2Move) {
    return TIE;
  }
  switch(p1Move) {
    case ROCK:
      if (p2Move == PAPER) {
        return PLAYERTWOWIN;
      }
      break;
    case PAPER:
      if (p2Move == SCISSOR) {
        return PLAYERTWOWIN;
      }
      break;
    case SCISSOR:
      if (p2Move == ROCK) {
        return PLAYERTWOWIN;
      }
      break;
    default:
      /* Impossible! */
      break;
  }
  return PLAYERONEWIN;
}