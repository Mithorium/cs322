#ifndef RPSTOURNAMENTMATCHFORMAT_H
#define RPSTOURNAMENTMATCHFORMAT_H

#include "common.h"
#include "RPSGame.h"

class RPSTournamentMatchFormat {
public:
  virtual ~RPSTournamentMatchFormat() {}
  virtual void playMatch(RPSGame &oRPSGame, RPSGame::Result &result) = 0;
  void getDescription(std::string &out) const {
    out.assign(description());
  }
  std::ostream &dump(std::ostream &o) const {
    std::string out;
    getDescription(out);
    return o << out;
  }
protected:
  virtual const char* description() const = 0;
};
inline std::ostream &operator<<(std::ostream &o, const RPSTournamentMatchFormat *p) { return p->dump(o); }

/** Factory class for RPSTournamentMatchFormat
  * Each player class is responsible for registering at the factory by using macros
  * DECLARE_RPSMATCHFORMAT(NAME)
  * in the class, and
  * REGISTER_RPSMATCHFORMAT(NAME)
  * outside
  */
class RPSTournamentMatchFormatFactory {
public:
  typedef std::unordered_map<std::string, RPSTournamentMatchFormat*(*)()> registry_type;
  static RPSTournamentMatchFormat *createRPSTournamentMatchFormat(std::string const &s) {
    registry_type::iterator it = registry().find(s);
    if (it == registry().end()) {
      throw std::runtime_error("Match Format not found.");
    }
    return it->second();
  }
  static void recycle(RPSTournamentMatchFormat *format) {
    delete format;
  }
protected:
  template<class T>
  static RPSTournamentMatchFormat* createT() {
    return new T;
  }
  static registry_type &registry() {
    static registry_type registry;
    return registry;
  }
};

template<class T>
class RPSTournamentMatchFormatRegister : RPSTournamentMatchFormatFactory {
public:
  RPSTournamentMatchFormatRegister(std::string const &s) {
    registry().insert(std::make_pair(s,&createT<T>));
  }
};

#define DECLARE_RPSMATCHFORMAT(NAME) static RPSTournamentMatchFormatRegister<NAME> reg
#define REGISTER_RPSMATCHFORMAT(NAME) RPSTournamentMatchFormatRegister<NAME> NAME::reg(#NAME)

#endif