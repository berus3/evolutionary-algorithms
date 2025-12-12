#pragma once
#ifndef COMBAT_ENGINE_HPP
#define COMBAT_ENGINE_HPP

#include <stdint.h>
#include "team.hpp"
#include "player.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip> 
#include <algorithm>
#include <array>
#include <map>


enum FightResult {
    TEAM1_WIN,
    TEAM2_WIN
};

void print_battlefield(Team* team1, Team* team2);

void chooseInstance(Instance i);

std::vector<double> winrate(std::vector<Team*> teams);

FightResult bo3(Team* team1, Team* team2);

// Returns true if the fight has ended
bool step(Team* team1, Team* team2);

FightResult fight(Team* team1, Team* team2);

int64_t runLoop(int64_t n);

#endif // COMBAT_ENGINE_HPP
