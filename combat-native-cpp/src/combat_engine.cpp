#include "../include/combat_engine.hpp"

extern Instance instance;


bool step(Team* team1, Team* team2) {
    team1->getPlayer(0)->act(team1, team2);
    team2->getPlayer(0)->act(team2, team1);
    team2->getPlayer(1)->act(team2, team1);
    team1->getPlayer(1)->act(team1, team2);
    team1->getPlayer(2)->act(team1, team2);
    team2->getPlayer(2)->act(team2, team1);
    team2->getPlayer(3)->act(team2, team1);
    team1->getPlayer(3)->act(team1, team2);
    team1->getPlayer(4)->act(team1, team2);
    team2->getPlayer(4)->act(team2, team1);

	return((team1->getPlayer(0)->isAlive() || team1->getPlayer(1)->isAlive() || team1->getPlayer(2)->isAlive() || team1->getPlayer(3)->isAlive() || team1->getPlayer(4)->isAlive()) 
		&& (team2->getPlayer(0)->isAlive() || team2->getPlayer(1)->isAlive() || team2->getPlayer(2)->isAlive() || team2->getPlayer(3)->isAlive() || team2->getPlayer(4)->isAlive()));
}



FightResult fight(Team* team1, Team* team2) {
    bool end = false;
    do {
        // make a step

    } while (!end);
    return DRAW;
}

int64_t runLoop(int64_t n) {
    int64_t sum = 0;
    for (int64_t i = 0; i < n; i++) sum += i;
    return sum;
}

void chooseInstance(Instance i) {
	instance = i;
}
