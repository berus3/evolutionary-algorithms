#include "../include/team.hpp"

Team::Team(int id) {
    team_id = id;
    for (int i = 0; i < 5; i++)
        players[i] = nullptr;
}

int Team::getId() {
    return team_id;
}

Player* Team::getPlayer(int index) {
    if (index < 0 || index >= 5)
        return nullptr;
    return players[index];
}

void Team::setPlayer(int index, Player* p) {
    if (index < 0 || index >= 5)
        return;
    players[index] = p;
}
