#include "../include/combat_engine.hpp"

extern Instance instance;

void print_battlefield(Team* team1, Team* team2) {
    std::cout << "Team " << team1->getId() << ":\n";
    for (int i = 0; i < 5; i++) {
        Player* p = team1->getPlayer(i);
        std::cout << " Player " << p->getId() << ": HP = " << p->getDynStats()->hp << (p->isAlive() ? " (Alive)" : " (Dead)") << " Last attacked: " << p->lastAttacked << "\n";
    }
    std::cout << "Team " << team2->getId() << ":\n";
    for (int i = 0; i < 5; i++) {
        Player* p = team2->getPlayer(i);
        std::cout << " Player " << p->getId() << ": HP = " << p->getDynStats()->hp << (p->isAlive() ? " (Alive)" : " (Dead)") << " Last attacked: " << p->lastAttacked << "\n";
    }
    std::cout << "-------------------------\n";
}

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

    print_battlefield(team1, team2);
	return((team1->getPlayer(0)->isAlive() || team1->getPlayer(1)->isAlive() || team1->getPlayer(2)->isAlive() || team1->getPlayer(3)->isAlive() || team1->getPlayer(4)->isAlive()) 
		&& (team2->getPlayer(0)->isAlive() || team2->getPlayer(1)->isAlive() || team2->getPlayer(2)->isAlive() || team2->getPlayer(3)->isAlive() || team2->getPlayer(4)->isAlive()));
}



FightResult fight(Team* team1, Team* team2) {
    bool end = false;
    int it = 0;
    do {
        end = !step(team1, team2);
        bool team1_alive = team1->getPlayer(0)->isAlive() || team1->getPlayer(1)->isAlive() || team1->getPlayer(2)->isAlive() || team1->getPlayer(3)->isAlive() || team1->getPlayer(4)->isAlive();
        bool team2_alive = team2->getPlayer(0)->isAlive() || team2->getPlayer(1)->isAlive() || team2->getPlayer(2)->isAlive() || team2->getPlayer(3)->isAlive() || team2->getPlayer(4)->isAlive();
        if (end) {
            if (team1_alive && !team2_alive)
                return TEAM1_WIN;
            else if (!team1_alive && team2_alive)
                return TEAM2_WIN;
            else
                return DRAW;
        }
        ++it;
    } while (!end);
    std::cout << "Fight ended in " << it << " iterations.\n";
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
