#include "../include/combat_engine.hpp"


extern Instance instance;
std::string hpBar(int hp, int maxHp, int width = 20) {
    float ratio = (maxHp > 0) ? (float)hp / maxHp : 0.0f;
    int filled = (int)(ratio * width);
    return "[" + std::string(filled, '#') + std::string(width - filled, '-') + "]";
}

std::array<std::pair<std::string, int>, 3> top3Stats(const StatPoints* sp) {
    std::vector<std::pair<std::string, int>> stats = {
        {"HP", sp->max_hp}, {"Regen", sp->regen}, {"AD", sp->ad},
        {"Armor", sp->armor}, {"ArmorPen", sp->armor_pen},
        {"Lethality", sp->lethality}, {"AS", sp->as}, {"Crit", sp->crit},
        {"CritFactor", sp->crit_factor}, {"Bleed", sp->bleed},
        {"BleedDmg", sp->bleed_dmg}, {"BleedTicks", sp->bleed_ticks},
        {"AP", sp->ap}, {"MR", sp->mr}, {"MRPen", sp->mr_pen},
        {"Ethereal", sp->ethereal}, {"AH", sp->ah}, {"Smite", sp->smite},
        {"SmiteCD", sp->cd_smite}, {"Blast", sp->blast},
        {"BlastCD", sp->cd_blast}, {"Heal", sp->heal},
        {"HealCD", sp->cd_heal}, {"Stun", sp->stun},
        {"StunCD", sp->cd_stun}, {"Acc", sp->acc},
        {"AccTicks", sp->acc_ticks}, {"AccCD", sp->cd_acc},
        {"Slow", sp->slow}, {"SlowTicks", sp->slow_ticks},
        {"SlowCD", sp->cd_slow}, {"Shield", sp->shield},
        {"ShieldTicks", sp->shield_ticks}, {"ShieldCD", sp->cd_shield},
        {"Mark", sp->mark}, {"MarkTicks", sp->mark_ticks},
        {"MarkCD", sp->cd_mark}, {"Vamp", sp->vamp}, {"Thorns", sp->thorns},
        {"AX", sp->ax}, {"Tenacity", sp->tenacity}, {"Aggro", sp->aggro},
        {"Focus", sp->focus}
    };

    std::sort(stats.begin(), stats.end(),
        [](auto& a, auto& b){ return a.second > b.second; });

    return { stats[0], stats[1], stats[2] };
}


void print_battlefield(Team* team1, Team* team2) {
    // Clear screen
    std::cout << "\033[2J\033[H";

    const int COL1  = 26;
    const int COL2  = 24; // HP bar
    const int COLE  = 30; // efectos
    const int COL3  = 6;  // estado
    const int COL4  = 6;  // lastAttacked
    const int COL5  = 18; // top stat 1
    const int COL6  = 18; // top stat 2
    const int COL7  = 18; // top stat 3
    const int COL8  = 12; // damage dealt
    const int COL9  = 12; // damage taken
    const int COL10 = 12; // heal done

    const char* RED          = "\033[31m";
    const char* GREEN        = "\033[92m";
    const char* YELLOW       = "\033[93m";
    const char* LIGHT_GRAY   = "\033[37m";  // o "\033[90m" si querés más oscuro
    const char* LIGHT_PURPLE = "\033[95m";
    const char* RESET        = "\033[0m";

    auto printTeam = [&](Team* t) {
        std::cout << "Team " << t->getId() << ":\n";

        for (int i = 0; i < 5; i++) {
            Player* p = t->getPlayer(i);
            if (!p) continue;

            int  hp    = p->getDynStats()->hp;
            int  maxHp = getStatMaxHp(p->getStatPoints()->max_hp);
            auto* ds   = p->getDynStats();
            auto* sp   = p->getStatPoints();

            std::string bar   = hpBar(hp, maxHp);
            std::string state = p->isAlive() ? "Alive" : "Dead";

            // Col1: etiqueta jugador + HP
            std::ostringstream col1;
            col1 << "Player " << p->getId() << "  HP " << hp << "/" << maxHp;

            // Col4: lastAttacked
            std::ostringstream col4;
            col4 << p->lastAttacked;

            // Top 3 stats
            auto top3 = top3Stats(sp);
            std::ostringstream t1, t2, t3;
            t1 << top3[0].first << "=" << top3[0].second;
            t2 << top3[1].first << "=" << top3[1].second;
            t3 << top3[2].first << "=" << top3[2].second;

            // Tracking
            std::ostringstream td, tr, th;
            td << "Dealt=" << ds->track_damage_dealt;
            tr << "Taken=" << ds->track_damage_received;
            th << "Heal="  << ds->track_hp_raw_healed;

            // Columna de efectos (texto)
            std::ostringstream effects;
            bool firstEff = true;
            auto addEff = [&](const std::string& s) {
                if (!firstEff) effects << " ";
                effects << s;
                firstEff = false;
            };

            if (ds->bleed_stacks > 0)
                addEff("BLD:" + std::to_string(ds->bleed_stacks));
            if (ds->end_stun > 0)
                addEff("STUN");
            if (ds->end_acc > 0)
                addEff("ACC");
            if (ds->end_slow > 0)
                addEff("SLOW");

            // ¿Fue atacado este tick?
            bool wasAttacked = false;
            for (int j = 0; j < 5; j++) {
                if (team1->getPlayer(j)->lastAttacked == p->getId()) wasAttacked = true;
                if (team2->getPlayer(j)->lastAttacked == p->getId()) wasAttacked = true;
            }

            // Padding del HP bar
            std::ostringstream pad;
            pad << std::left << std::setw(COL2) << bar;
            std::string paddedBar = pad.str();

            // Elegir color por prioridad:
            // 1) STUN > 2) HEAL THIS TICK > 3) ACC > 4) SLOW > 5) WAS ATTACKED
            const char* color = nullptr;
            if (ds->end_stun > 0) {
                color = LIGHT_PURPLE;
            } else if (ds->track_healed_this_tick) {
                color = GREEN;
            } else if (ds->end_acc > 0) {
                color = YELLOW;
            } else if (ds->end_slow > 0) {
                color = LIGHT_GRAY;
            } else if (wasAttacked) {
                color = RED;
            }

            std::string coloredBar;
            if (color)
                coloredBar = std::string(color) + paddedBar + RESET;
            else
                coloredBar = paddedBar;

            // Output final de la fila
            std::cout
                << std::left << std::setw(COL1)  << col1.str()
                << coloredBar
                << std::setw(COLE)  << effects.str()
                << std::setw(COL3)  << state
                << std::setw(COL4)  << col4.str()
                << std::setw(COL5)  << t1.str()
                << std::setw(COL6)  << t2.str()
                << std::setw(COL7)  << t3.str()
                << std::setw(COL8)  << td.str()
                << std::setw(COL9)  << tr.str()
                << std::setw(COL10) << th.str()
                << "\n";
        }
    };

    printTeam(team1);
    printTeam(team2);

    std::cout << "------------------------------------------------------------\n";
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
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
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

