#pragma once
#include <cstddef>

constexpr int STATS_PER_PLAYER     = 43;
constexpr int PLAYERS_PER_TEAM     = 5;
constexpr int DECISIONS_PER_PLAYER = 100;
constexpr int GENOME_SIZE          = DECISIONS_PER_PLAYER * PLAYERS_PER_TEAM; // 500

constexpr int ANCHOR_COUNT = 5;

extern const int ANCHOR_GENOMES[ANCHOR_COUNT][GENOME_SIZE];
