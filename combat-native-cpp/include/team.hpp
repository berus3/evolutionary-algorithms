#ifndef TEAM_HPP
#define TEAM_HPP

#include "player.hpp"

class Team {
  private:
    int _team_id;
    Player* _players[5];
  public:
    Team(int id);
    ~Team();
    int getId();
    Player* getPlayer(int index);
    void setPlayer(int index, Player* p);
};


#endif // TEAM_HPP
