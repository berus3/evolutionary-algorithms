#ifndef TEAM_HPP
#define TEAM_HPP
#include "player.hpp"

class Team {
  private:
	int team_id;
	Player* players[5];

  public:
    Team(int id);
    int getId();
    Player* getPlayer(int index);
    void setPlayer(int index, Player* p);
};


#endif // TEAM_HPP
