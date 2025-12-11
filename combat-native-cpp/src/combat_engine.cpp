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
        {"AccTicks", sp->acc_as_ticks}, {"AccCD", sp->cd_acc},
        {"Slow", sp->slow}, {"SlowTicks", sp->slow_as_ticks},
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
    std::cout << "\033[2J\033[H"; // clear console

    const int COL1 = 26;   // Player + HP text
    const int COL2 = 24;   // HP bar
    const int COL3 = 6;    // Alive/Dead
    const int COL4 = 6;    // Last attacked
    const int COL5 = 18;   // top stat 1
    const int COL6 = 18;   // top stat 2
    const int COL7 = 18;   // top stat 3
    const int COL8 = 12;   // damage dealt
    const int COL9 = 12;   // damage received
    const int COL10 = 12;  // hp healed

    const char* RED   = "\033[31m";
    const char* RESET = "\033[0m";

    auto printTeam = [&](Team* t) {
        std::cout << "Team " << t->getId() << ":\n";

        for (int i = 0; i < 5; i++) {
            Player* p = t->getPlayer(i);

            int hp     = p->getDynStats()->hp;
            int maxHp  = getStatMaxHp(p->getStatPoints()->max_hp);

            std::string bar   = hpBar(hp, maxHp);
            std::string state = p->isAlive() ? "Alive" : "Dead";

            // Column 1: Player + HP
            std::ostringstream col1;
            col1 << "Player " << p->getId()
                 << "  HP " << hp << "/" << maxHp;

            // Column 4: Last attacked
            std::ostringstream col4;
            col4 << p->lastAttacked;

            // Top 3 stats
            auto top3 = top3Stats(p->getStatPoints());

            std::ostringstream t1, t2, t3;
            t1 << top3[0].first << "=" << top3[0].second;
            t2 << top3[1].first << "=" << top3[1].second;
            t3 << top3[2].first << "=" << top3[2].second;

            // Damage tracking columns
            int dmgDealt    = p->getDynStats()->track_damage_dealt;
            int dmgTaken    = p->getDynStats()->track_damage_received;
            int hpHealed    = p->getDynStats()->track_hp_healed;

            std::ostringstream td, tr, th;
            td << "Dealt="   << dmgDealt;
            tr << "Taken=" << dmgTaken;
            th << "Healed="  << hpHealed;
            
            bool wasAttacked = false;
            for (int j = 0; j < 5; j++) {
                if (team1->getPlayer(j)->lastAttacked == p->getId()) wasAttacked = true;
                if (team2->getPlayer(j)->lastAttacked == p->getId()) wasAttacked = true;
            }

            // Pad bar BEFORE adding ANSI color to prevent alignment issues
            std::ostringstream pad;
            pad << std::left << std::setw(COL2) << bar;
            std::string paddedBar = pad.str();

            // Only color the bar characters, not the padding
            std::string coloredBar;
            if (wasAttacked) {
                coloredBar =
                    std::string(RED) +
                    paddedBar.substr(0, bar.size()) +
                    RESET +
                    paddedBar.substr(bar.size());
            } else {
                coloredBar = paddedBar;
            }

            // Print final row
            std::cout
                << std::left << std::setw(COL1)  << col1.str()
                << coloredBar
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
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
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

