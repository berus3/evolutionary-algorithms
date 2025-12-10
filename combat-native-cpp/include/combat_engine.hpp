#pragma once
#ifndef COMBAT_ENGINE_HPP
#define COMBAT_ENGINE_HPP

#include <stdint.h>
#include "team.hpp"
#include "player.hpp"

enum FightResult {
    TEAM1_WIN,
    TEAM2_WIN,
    DRAW
};


// Returns true if the fight has ended
void chooseInstance(Instance i);

bool step(Team* team1, Team* team2);

FightResult fight(Team* team1, Team* team2);

int64_t runLoop(int64_t n);

#endif // COMBAT_ENGINE_HPP