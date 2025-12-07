#include "team.hpp"

Team::Team(int id) { this->team_id = id; }
int Team::getId() { return team_id; }
Player* Team::getPlayer1() { return player1; }
Player* Team::getPlayer2() { return player2; }
Player* Team::getPlayer3() { return player3; }
Player* Team::getPlayer4() { return player4; }
Player* Team::getPlayer5() { return player5; }
void Team::setPlayer1(Player* p) { player1 = p; }
void Team::setPlayer2(Player* p) { player2 = p; }
void Team::setPlayer3(Player* p) { player3 = p; }
void Team::setPlayer4(Player* p) { player4 = p; }
void Team::setPlayer5(Player* p) { player5 = p; }