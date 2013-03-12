#include "Players.h"

RPSGame::Move HumanRPSPlayer::chooseMove() {
  std::string input;
  getName(input);
  std::cout << input << ": Choose a move: (r/p/s): ";
  for (;;) {
    std::getline(std::cin,input);
    if (input.size() > 0 && *input.rbegin() == '\r') {
      input.erase(input.end()-1);
    }
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    if (input.compare("r") == 0) {
      return RPSGame::ROCK;
    } else if (input.compare("p") == 0) {
      return RPSGame::PAPER;
    } else if (input.compare("s") == 0) {
      return RPSGame::SCISSOR;
    }
    std::cout << "Please enter r, p or s: ";
  }
  return RPSGame::PAPER;
}
void HumanRPSPlayer::onRound(RPSGame::Move myMove, RPSGame::Move oppMove) {
  std::cout << "Opponent played: ";
  switch(oppMove) {
    case RPSGame::ROCK:
      std::cout << "Rock\n";
      break;
    case RPSGame::PAPER:
      std::cout << "Paper\n";
      break;
    case RPSGame::SCISSOR:
      std::cout << "Scissor\n";
      break;
    default:
      break;
  }
}
/* We do not want to add humans to the factory pool as multiple instances of each player is made */
// REGISTER_RPSPLAYER(HumanRPSPlayer);

RPSGame::Move RockRPSPlayer::chooseMove() {
  // always choose rock
  return RPSGame::ROCK;
}
REGISTER_RPSPLAYER(RockRPSPlayer);

BeatOpponentsPreviousMoveRPSPlayer::BeatOpponentsPreviousMoveRPSPlayer() {
  prevMove = RPSGame::NONE;
}
RPSGame::Move BeatOpponentsPreviousMoveRPSPlayer::chooseMove() {
  switch (prevMove) {
    case RPSGame::ROCK:
      return RPSGame::PAPER;
      break;
    case RPSGame::PAPER:
      return RPSGame::SCISSOR;
      break;
    case RPSGame::SCISSOR:
      return RPSGame::ROCK;
      break;
    default:
      unsigned char choice = rand() % 3;
      if (choice == 0) {
        return RPSGame::ROCK;
      } else if (choice == 1) { 
        return RPSGame::PAPER;
      } else {
        return RPSGame::SCISSOR;
      }
      break;
  }
}
void BeatOpponentsPreviousMoveRPSPlayer::onRound(RPSGame::Move myMove, RPSGame::Move oppMove) {
  prevMove = oppMove;
}
void BeatOpponentsPreviousMoveRPSPlayer::onNewOpponent() {
  prevMove = RPSGame::NONE;
}
REGISTER_RPSPLAYER(BeatOpponentsPreviousMoveRPSPlayer);

RPSGame::Move RandomRPSPlayer::chooseMove() {
  unsigned char choice = rand() % 3;
  switch (choice) {
    case 0:
      return RPSGame::ROCK;
    case 1:
      return RPSGame::PAPER;
    case 2:
      return RPSGame::SCISSOR;
    default:
      throw std::runtime_error("Impossible error!");
      break;
  }
}
REGISTER_RPSPLAYER(RandomRPSPlayer);