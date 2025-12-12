#include <jni.h>
#include <vector>
#include <iostream>

#include "org_evol_RPGNativeBridge.h"
#include "combat_engine.hpp"
#include "team.hpp"
#include "player.hpp"

/* ============================================================
   CONSTANTES DE LA CODIFICACIÓN GENÉTICA
   ============================================================ */
static constexpr int STATS_PER_PLAYER      = 43;
static constexpr int PLAYERS_PER_TEAM      = 5;
static constexpr int DECISIONS_PER_PLAYER  = 100;
static constexpr int GENOME_SIZE           = DECISIONS_PER_PLAYER * PLAYERS_PER_TEAM;
// = 500 para cada individuo


/* ============================================================
   Construir un Team* desde una tira de 500 genes
   ============================================================ */
Team* buildTeamFromGenome(const int* genome) {
    Team* team = new Team(-1);

    for (int p = 0; p < PLAYERS_PER_TEAM; p++) {

        Player* player = new Player(p);
        StatPoints* sp = new StatPoints();

        /* buckets[stat] = cantidad de puntos gastados por el jugador p */
        int buckets[STATS_PER_PLAYER] = {0};

        int start = p * DECISIONS_PER_PLAYER;

        for (int i = 0; i < DECISIONS_PER_PLAYER; i++) {
            int statId = genome[start + i];
            if (statId >= 0 && statId < STATS_PER_PLAYER) {
                buckets[statId] += 1;
            }
        }

        /* Mapear los buckets a la estructura StatPoints */
        sp->max_hp        = buckets[0];
        sp->regen         = buckets[1];
        sp->ad            = buckets[2];
        sp->armor         = buckets[3];
        sp->armor_pen     = buckets[4];
        sp->lethality     = buckets[5];
        sp->as            = buckets[6];
        sp->crit          = buckets[7];
        sp->crit_factor   = buckets[8];
        sp->bleed         = buckets[9];
        sp->bleed_dmg     = buckets[10];
        sp->bleed_ticks   = buckets[11];
        sp->ap            = buckets[12];
        sp->mr            = buckets[13];
        sp->mr_pen        = buckets[14];
        sp->ethereal      = buckets[15];
        sp->ah            = buckets[16];
        sp->smite         = buckets[17];
        sp->cd_smite      = buckets[18];
        sp->blast         = buckets[19];
        sp->cd_blast      = buckets[20];
        sp->heal          = buckets[21];
        sp->cd_heal       = buckets[22];
        sp->stun          = buckets[23];
        sp->cd_stun       = buckets[24];
        sp->acc           = buckets[25];
        sp->acc_ticks     = buckets[26];
        sp->cd_acc        = buckets[27];
        sp->slow          = buckets[28];
        sp->slow_ticks    = buckets[29];
        sp->cd_slow       = buckets[30];
        sp->shield        = buckets[31];
        sp->shield_ticks  = buckets[32];
        sp->cd_shield     = buckets[33];
        sp->mark          = buckets[34];
        sp->mark_ticks    = buckets[35];
        sp->cd_mark       = buckets[36];
        sp->vamp          = buckets[37];
        sp->thorns        = buckets[38];
        sp->ax            = buckets[39];
        sp->tenacity      = buckets[40];
        sp->aggro         = buckets[41];
        sp->focus         = buckets[42];

        player->setStatPoints(sp);
        player->_init_player();

        team->setPlayer(p, player);
    }

    return team;
}


/* ============================================================
   JNI evaluatePopulation — Round-robin y winrates
   ============================================================ */

extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_org_evol_RPGNativeBridge_evaluatePopulation
(
    JNIEnv* env,
    jclass,
    jintArray flatPopulation,
    jint populationSize
) {
    jsize totalLen = env->GetArrayLength(flatPopulation);

    if (totalLen != populationSize * GENOME_SIZE) {
        jdoubleArray out = env->NewDoubleArray(populationSize);
        std::vector<jdouble> zeros(populationSize, 0.0);
        env->SetDoubleArrayRegion(out, 0, populationSize, zeros.data());
        return out;
    }

    jint* data = env->GetIntArrayElements(flatPopulation, nullptr);

    /* Construcción de equipos */
    std::vector<Team*> teams;
    teams.reserve(populationSize);

    for (int i = 0; i < populationSize; i++) {
        int* genome_i = data + (i * GENOME_SIZE);
        Team* t = buildTeamFromGenome(genome_i);
        t->setId(i);
        teams.push_back(t);
    }

    /* Round-robin */
    std::vector<jdouble> wins(populationSize, 0.0);

    for (int i = 0; i < populationSize; i++) {
        for (int j = i + 1; j < populationSize; j++) {
            FightResult r = bo3(teams[i], teams[j]);
            if (r == TEAM1_WIN) wins[i]++;
            else wins[j]++;
        }
    }

    /* Winrate */
    std::vector<jdouble> winrate(populationSize, 0.0);
    double totalMatches = (populationSize - 1);

    for (int i = 0; i < populationSize; i++) {
        winrate[i] = wins[i] / totalMatches;
    }

    /* Output JNI */
    jdoubleArray out = env->NewDoubleArray(populationSize);
    env->SetDoubleArrayRegion(out, 0, populationSize, winrate.data());

    env->ReleaseIntArrayElements(flatPopulation, data, 0);

    for (Team* t : teams) delete t;

    return out;
}
