#include <jni.h>
#include <vector>
#include <iostream>
#include "org_evol_RPGNativeBridge.h"
#include "combat_engine.hpp"
#include "team.hpp"
#include "player.hpp"

static constexpr int STATS_PER_PLAYER = 43;
static constexpr int PLAYERS_PER_TEAM = 5;
static constexpr int GENOME_SIZE = STATS_PER_PLAYER * PLAYERS_PER_TEAM;


Team* buildTeamFromGenome(const int* genome) {
    Team* team = new Team(-1);
    for (int p = 0; p < PLAYERS_PER_TEAM; p++) {
        Player* player = new Player(p);
        StatPoints* sp = new StatPoints();
        int offset = p * STATS_PER_PLAYER;
        sp->max_hp        = genome[offset +  0];
        sp->regen         = genome[offset +  1];
        sp->ad            = genome[offset +  2];
        sp->armor         = genome[offset +  3];
        sp->armor_pen     = genome[offset +  4];
        sp->lethality     = genome[offset +  5];
        sp->as            = genome[offset +  6];
        sp->crit          = genome[offset +  7];
        sp->crit_factor   = genome[offset +  8];
        sp->bleed         = genome[offset +  9];
        sp->bleed_dmg     = genome[offset + 10];
        sp->bleed_ticks   = genome[offset + 11];
        sp->ap            = genome[offset + 12];
        sp->mr            = genome[offset + 13];
        sp->mr_pen        = genome[offset + 14];
        sp->ethereal      = genome[offset + 15];
        sp->ah            = genome[offset + 16];
        sp->smite         = genome[offset + 17];
        sp->cd_smite      = genome[offset + 18];
        sp->blast         = genome[offset + 19];
        sp->cd_blast      = genome[offset + 20];
        sp->heal          = genome[offset + 21];
        sp->cd_heal       = genome[offset + 22];
        sp->stun          = genome[offset + 23];
        sp->cd_stun       = genome[offset + 24];
        sp->acc           = genome[offset + 25];
        sp->acc_ticks     = genome[offset + 26];
        sp->cd_acc        = genome[offset + 27];
        sp->slow          = genome[offset + 28];
        sp->slow_ticks    = genome[offset + 29];
        sp->cd_slow       = genome[offset + 30];
        sp->shield        = genome[offset + 31];
        sp->shield_ticks  = genome[offset + 32];
        sp->cd_shield     = genome[offset + 33];
        sp->mark          = genome[offset + 34];
        sp->mark_ticks    = genome[offset + 35];
        sp->cd_mark       = genome[offset + 36];
        sp->vamp          = genome[offset + 37];
        sp->thorns        = genome[offset + 38];
        sp->ax            = genome[offset + 39];
        sp->tenacity      = genome[offset + 40];
        sp->aggro         = genome[offset + 41];
        sp->focus         = genome[offset + 42];
        player->setStatPoints(sp);
        player->_init_player();
        team->setPlayer(p, player);
    }

    return team;
}

extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_org_evol_RPGNativeBridge_evaluatePopulation(JNIEnv* env, jclass, jintArray flatPopulation, jint populationSize) {
    jsize totalLen = env->GetArrayLength(flatPopulation);
    if (totalLen != populationSize * GENOME_SIZE) {
        jdoubleArray out = env->NewDoubleArray(populationSize);
        std::vector<jdouble> zeros(populationSize, 0.0);
        env->SetDoubleArrayRegion(out, 0, populationSize, zeros.data());
        return out;
    }
    jint* data = env->GetIntArrayElements(flatPopulation, nullptr);
    std::vector<Team*> teams;
    teams.reserve(populationSize);
    for (int i = 0; i < populationSize; i++) {
        int* genome_i = data + (i * GENOME_SIZE);
        Team* t = buildTeamFromGenome(genome_i);
        t->setId(i);
        teams.push_back(t);
    }
    std::vector<jdouble> wins(populationSize, 0.0);
    for (int i = 0; i < populationSize; i++) {
        for (int j = i + 1; j < populationSize; j++) {
            FightResult r = bo3(teams[i], teams[j]);
            if (r == TEAM1_WIN)
                wins[i]++;
            else
                wins[j]++;
        }
    }
    std::vector<jdouble> winrate(populationSize, 0.0);
    double totalMatches = (populationSize - 1);
    for (int i = 0; i < populationSize; i++) {
        winrate[i] = wins[i] / totalMatches;
    }
    jdoubleArray out = env->NewDoubleArray(populationSize);
    env->SetDoubleArrayRegion(out, 0, populationSize, winrate.data());
    env->ReleaseIntArrayElements(flatPopulation, data, 0);
    for (Team* t : teams)
        delete t;
    return out;
}