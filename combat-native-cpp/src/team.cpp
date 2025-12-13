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


Team::Team(const Team& o) {
    _team_id = o._team_id;
    for (int i = 0; i < 5; ++i) {
        if (o._players[i])
            _players[i] = o._players[i]->clone();
        else
            _players[i] = nullptr;
    }
}

Team Team::clone() const {
    return Team(*this);
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
