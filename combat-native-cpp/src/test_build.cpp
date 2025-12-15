#include <iostream>

#include "anchors.hpp"
#include "genome_decode.hpp"
#include "team.hpp"
#include "player.hpp"

// print player (esto está bien dejarlo)
static void printPlayerStats(Player* p) {
    StatPoints* sp = p->getStatPoints();
    int* arr = reinterpret_cast<int*>(sp);

    std::cout << "---- PLAYER " << p->getId() << " ----\n";
    int sum = 0;

    for (int i = 0; i < STATS_PER_PLAYER; ++i) {
        std::cout << "stat[" << i << "] = " << arr[i] << "\n";
        sum += arr[i];
    }

    std::cout << "TOTAL POINTS = " << sum << "\n\n";
}

int main() {
    const int* genome = ANCHOR_GENOMES[0];

    Team* t = buildTeamFromGenome(genome, 0);

    std::cout << "\n=========================\n";
    std::cout << "   RESULTADO DEL TEAM\n";
    std::cout << "=========================\n\n";

    for (int p = 0; p < PLAYERS_PER_TEAM; ++p) {
        printPlayerStats(t->getPlayer(p));
    }

    delete t;
    return 0;
}
