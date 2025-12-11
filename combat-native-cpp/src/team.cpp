#include "../include/team.hpp"

Team::Team(int id) {
    _team_id = id;
    for (int i = 0; i < 5; i++)
        _players[i] = nullptr;
}

Team::~Team() {
    for (int i = 0; i < 5; i++)
        delete _players[i];
}

int Team::getId() {
    return _team_id;
}

Player* Team::getPlayer(int index) {
    if (index < 0 || index >= 5)
        return nullptr;
    return _players[index];
}

void Team::setPlayer(int index, Player* p) {
    if (index < 0 || index >= 5)
        return;
    _players[index] = p;
}
