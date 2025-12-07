#pragma once
#include <stdint.h>
#include "team.hpp"
#include "player.hpp"

enum FightResult {
    TEAM1_WIN,
    TEAM2_WIN,
    DRAW
};

enum Instance {
    BALANCED,
    LINEAR,
    UNFAIR
};

// Returns true if the fight has ended
bool step(Team* team1, Team* team2, Instance instance);

FightResult fight(Team* team1, Team* team2, Instance instance);

int64_t runLoop(int64_t n);