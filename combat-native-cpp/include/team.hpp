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
    void setId(int id) { _team_id = id; }
    Player* getPlayer(int index);
    void setPlayer(int index, Player* p);
    Team(const Team& other);   
    Team clone() const;        
    
};


#endif // TEAM_HPP
