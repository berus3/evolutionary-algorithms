#include "combat_engine.hpp"

bool step(Team* team1, Team* team2, Instance instance) {
    team1->getPlayer1()->act(team1, team2);
    team2->getPlayer1()->act(team2, team1);
    team2->getPlayer2()->act(team2, team1);
    team1->getPlayer2()->act(team1, team2);
    team1->getPlayer3()->act(team1, team2);
    team2->getPlayer3()->act(team2, team1);
    team2->getPlayer4()->act(team2, team1);
    team1->getPlayer4()->act(team1, team2);
    team1->getPlayer5()->act(team1, team2);
    team2->getPlayer5()->act(team2, team1);
}

FightResult fight(Team* team1, Team* team2, Instance instance) {
    bool end = false;
    do {
        // make a step

    } while (!end);
}

int64_t runLoop(int64_t n) {
    int64_t sum = 0;
    for (int64_t i = 0; i < n; i++) sum += i;
    return sum;
}
