#include "RPSTournament.h"
#include "Players.h"

RPSTournament::RPSTournament(unsigned int numPlayers) {
  if (numPlayers == 0) {
    throw std::invalid_argument("numPlayers must be greater than 0");
  }
  roster.reserve(numPlayers);
  initPlayers(numPlayers);
}

RPSTournament::~RPSTournament() {
  while (!roster.empty()) {
    RPSPlayerFactory::recycle(roster.back());
    roster.pop_back();
  }
}

void RPSTournament::play() {
  unsigned int round = 0;
  unsigned int size;
  std::cout << "Tournament format: Best of 1 (Sudden Death) w/ up to 2 tiebreaker matches\n";
  while ((size = (unsigned int)roster.size()) > 1) {
    std::cout << "Round " << ++round << ":\n"
              << "Competitors: " << size << "\n";
    roster_type players;
    roster.swap(players);
    roster.reserve(ceil(size/2.0));
    while (players.size() > 1) {
      /* Choose two players at random */
      RPSPlayer *p1 = Util::pop_random(players);
      RPSPlayer *p2 = Util::pop_random(players);
      RPSGame oRPSGame(p1,p2);
      
      unsigned char rounds = 3;
      RPSGame::Result result;
      do {
        result = oRPSGame.playRound();
      } while (--rounds && result == RPSGame::TIE);
      switch (result) {
        case RPSGame::PLAYERONEWIN:
          std::cout << p1 << " (Win) vs. " << p2 << "\n";
          advance(p1);
          eliminate(p2);
          break;
        case RPSGame::PLAYERTWOWIN:
          std::cout << p1 << " vs. " << p2 << " (Win)\n";
          advance(p2);
          eliminate(p1);
          break;
        case RPSGame::TIE:
          std::cout << p1 << " vs. " << p2 << " (Tie)\n";
          eliminate(p1);
          eliminate(p2);
          break;
        default:
          /* Impossible */
          break;
      }
    }
    if (players.size() == 1) {
      advance(players.back());
      players.pop_back();
    }
  }
  if (roster.size() == 1) {
    std::string name;
    roster.back()->getName(name);
    std::cout << "Winner: " << roster.back() << "\n";
  } else {
    std::cout << "All competitors have been eliminated!\n";
  }
}

void RPSTournament::advance(RPSPlayer *winner) {
  roster.push_back(winner);
}

void RPSTournament::eliminate(RPSPlayer *loser) {
  RPSPlayerFactory::recycle(loser);
}

void RPSTournament::initPlayers(unsigned int numPlayers) {
  std::string input;
  std::cout << "Add human player? (y/n): ";
  for (;;) {
    std::getline(std::cin,input);
    if (input.size() > 0 && *input.rbegin() == '\r') {
      input.erase(input.end()-1);
    }
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    if (input.compare("y") == 0) {
      roster.push_back((RPSPlayer *)new HumanRPSPlayer);
      --numPlayers;
      break;
    } else if (input.compare("n") == 0) {
      break;
    }
    std::cout << "Please enter y or n: ";
  }
  do {
    roster.push_back(RPSPlayerFactory::createRPSPlayer());
  } while(--numPlayers);
}