#include "../include/genome_decode.hpp"
#include "player.hpp"

static constexpr int STATS_PER_PLAYER = 43;
static constexpr int PLAYERS_PER_TEAM = 5;
static constexpr int DECISIONS_PER_PLAYER = 100;

Team* buildTeamFromGenome(const int* genome, int teamId) {
    Team* team = new Team(teamId);

    for (int p = 0; p < PLAYERS_PER_TEAM; ++p) {
        Player* player = new Player(p);

        delete player->getStatPoints();
        StatPoints* sp = new StatPoints();

        int buckets[STATS_PER_PLAYER] = {0};
        int start = p * DECISIONS_PER_PLAYER;

        for (int i = 0; i < DECISIONS_PER_PLAYER; ++i) {
            int s = genome[start + i];
            if (s >= 0 && s < STATS_PER_PLAYER)
                buckets[s]++;
        }

        sp->max_hp = buckets[0];
        sp->regen = buckets[1];
        sp->ad = buckets[2];
        sp->armor = buckets[3];
        sp->armor_pen = buckets[4];
        sp->lethality = buckets[5];
        sp->as = buckets[6];
        sp->crit = buckets[7];
        sp->crit_factor = buckets[8];
        sp->bleed = buckets[9];
        sp->bleed_dmg = buckets[10];
        sp->bleed_ticks = buckets[11];
        sp->ap = buckets[12];
        sp->mr = buckets[13];
        sp->mr_pen = buckets[14];
        sp->ethereal = buckets[15];
        sp->ah = buckets[16];
        sp->smite = buckets[17];
        sp->cd_smite = buckets[18];
        sp->blast = buckets[19];
        sp->cd_blast = buckets[20];
        sp->heal = buckets[21];
        sp->cd_heal = buckets[22];
        sp->stun = buckets[23];
        sp->cd_stun = buckets[24];
        sp->acc = buckets[25];
        sp->acc_ticks = buckets[26];
        sp->cd_acc = buckets[27];
        sp->slow = buckets[28];
        sp->slow_ticks = buckets[29];
        sp->cd_slow = buckets[30];
        sp->shield = buckets[31];
        sp->shield_ticks = buckets[32];
        sp->cd_shield = buckets[33];
        sp->mark = buckets[34];
        sp->mark_ticks = buckets[35];
        sp->cd_mark = buckets[36];
        sp->vamp = buckets[37];
        sp->thorns = buckets[38];
        sp->ax = buckets[39];
        sp->tenacity = buckets[40];
        sp->aggro = buckets[41];
        sp->focus = buckets[42];

        player->setStatPoints(sp);
        player->init_player();
        team->setPlayer(p, player);
    }

    return team;
}