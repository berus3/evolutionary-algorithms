#ifndef TEAM_HPP
#define TEAM_HPP
#include "player.hpp"

class Team {
  private:
      int team_id;
      Player* player1;
      Player* player2;
      Player* player3;
      Player* player4;
      Player* player5;
  public:
      Team(int id);
      int getId();
      Player* getPlayer1();
      Player* getPlayer2();
      Player* getPlayer3();
      Player* getPlayer4();
      Player* getPlayer5();
      void setPlayer1(Player* p);
      void setPlayer2(Player* p);
      void setPlayer3(Player* p);
      void setPlayer4(Player* p);
      void setPlayer5(Player* p);
};


#endif // TEAM_HPP