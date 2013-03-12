#ifndef RPSPLAYER_H
#define RPSPLAYER_H

#include "common.h"
#include "RPSGame.h"

/** abstract class RPSPlayer
 *  Each RPSPlayer can assume they will only be competing in one RPSGame at a time.
 *  As such, RPSPlayers should not be placed in multuple concurrent RPSGames.
 */
class RPSPlayer {
public:
  RPSPlayer() {
    static int n = 0;
    id = n;
    ++n;
  }
  virtual ~RPSPlayer() {}
  virtual RPSGame::Move chooseMove() = 0;
  virtual void onRound(RPSGame::Move myMove, RPSGame::Move oppMove) {
    /* Implement if player wants notification of what has been played */
  }
  virtual void onNewOpponent() {
    /* Implement if player wants notification when a different opponent is being played */
  }
  void getName(std::string &out) const {
    out.assign(name());
    out += std::to_string(id);
  }
  std::ostream &dump(std::ostream &o) const {
    std::string out;
    getName(out);
    return o << out;
  }
protected:
  virtual const char* name() const = 0;
private:
  int id;
};
inline std::ostream &operator<<(std::ostream &o, const RPSPlayer *p) { return p->dump(o); }

/** Factory class for RPSPlayer
  * Each player class is responsible for registering at the factory by using macros
  * DECLARE_RPSPLAYER(NAME)
  * in the class, and
  * REGISTER_RPSPLAYER(NAME)
  * outside
  */
class RPSPlayerFactory {
public:
  typedef std::vector<RPSPlayer*(*)()> registry_type;
  static RPSPlayer *createRPSPlayer() {
    static unsigned int next = 0;
    if (registry().size() == 0) {
      throw std::runtime_error("No player classes registered.");
    }
    if (next >= registry().size()) {
      next = 0;
    }
    return registry().at(next++)();
  }
  static void recycle(RPSPlayer *player) {
    delete player;
  }
protected:
  template<class T>
  static RPSPlayer* createT() {
    return new T;
  }
  static registry_type &registry() {
    static registry_type registry;
    return registry;
  }
};

template<class T>
class RPSPlayerRegister : RPSPlayerFactory {
public:
  RPSPlayerRegister() {
    registry().push_back(&createT<T>);
  }
};

#define DECLARE_RPSPLAYER(NAME) static RPSPlayerRegister<NAME> reg
#define REGISTER_RPSPLAYER(NAME) RPSPlayerRegister<NAME> NAME::reg

#endif