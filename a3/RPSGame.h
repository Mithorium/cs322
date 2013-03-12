#ifndef RPSGAME_H
#define RPSGAME_H

class RPSPlayer; /* Forward declare */

class RPSGame {
public:
  enum Move {NONE, ROCK, PAPER, SCISSOR};
  enum Result {PLAYERONEWIN, PLAYERTWOWIN, TIE};
  RPSGame(RPSPlayer *playerOne, RPSPlayer *playerTwo);
  Result play(unsigned short int rounds = 1);
  Result playRound();
private:
  Result evaluateMove(Move p1Move, Move p2Move);
  RPSPlayer *p1;
  RPSPlayer *p2;
};

#endif