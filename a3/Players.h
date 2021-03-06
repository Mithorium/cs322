#ifndef PLAYERS_H
#define PLAYERS_H

#include "common.h"
#include "RPSPlayer.h"
#include "RPSGame.h"

class HumanRPSPlayer : public RPSPlayer {
public:
  RPSGame::Move chooseMove();
  void onRound(RPSGame::Move myMove, RPSGame::Move oppMove);
protected:
  const char* name() const { return "HumanRPSPlayer"; }
private:
  // DECLARE_RPSPLAYER(HumanRPSPlayer);
};

class RockRPSPlayer : public RPSPlayer {
public:
  RPSGame::Move chooseMove();
protected:
  const char* name() const { return "RockRPSPlayer"; }
private:
  DECLARE_RPSPLAYER(RockRPSPlayer);
};

class PaperRPSPlayer : public RPSPlayer {
public:
  RPSGame::Move chooseMove();
protected:
  const char* name() const { return "PaperRPSPlayer"; }
private:
  DECLARE_RPSPLAYER(PaperRPSPlayer);
};

class ScissorRPSPlayer : public RPSPlayer {
public:
  RPSGame::Move chooseMove();
protected:
  const char* name() const { return "ScissorRPSPlayer"; }
private:
  DECLARE_RPSPLAYER(ScissorRPSPlayer);
};

class BeatOpponentsPreviousMoveRPSPlayer : public RPSPlayer {
public:
  BeatOpponentsPreviousMoveRPSPlayer();
  RPSGame::Move chooseMove();
  void onRound(RPSGame::Move myMove, RPSGame::Move oppMove);
  void onNewOpponent();
protected:
  const char* name() const { return "BeatOpponentsPreviousMoveRPSPlayer"; }
private:
  RPSGame::Move prevMove;
  DECLARE_RPSPLAYER(BeatOpponentsPreviousMoveRPSPlayer);
};

class RandomRPSPlayer : public RPSPlayer {
public:
  RPSGame::Move chooseMove();
protected:
  const char* name() const { return "RandomRPSPlayer"; }
private:
  DECLARE_RPSPLAYER(RandomRPSPlayer);
};

class SequentialRPSPlayer : public RPSPlayer {
public:
  SequentialRPSPlayer();
  RPSGame::Move chooseMove();
protected:
  const char* name() const { return "SequentialRPSPlayer"; }
private:
  unsigned char counter;
  DECLARE_RPSPLAYER(SequentialRPSPlayer);
};

class ReverseSequentialRPSPlayer : public RPSPlayer {
public:
  ReverseSequentialRPSPlayer();
  RPSGame::Move chooseMove();
protected:
  const char* name() const { return "ReverseSequentialRPSPlayer"; }
private:
  unsigned char counter;
  DECLARE_RPSPLAYER(ReverseSequentialRPSPlayer);
};

#endif