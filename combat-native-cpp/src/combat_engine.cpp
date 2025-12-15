#include "../include/combat_engine.hpp"
#include <vector>
#include <unordered_set>
#include <cstddef> // size_t
#include <omp.h>
#include <algorithm>

extern Instance instance;
std::string hpBar(int hp, int maxHp, int width = 20) {
    float ratio = (maxHp > 0) ? (float)hp / maxHp : 0.0f;
    int filled = (int)(ratio * width);
    return "[" + std::string(filled, '#') + std::string(width - filled, '-') + "]";
}

const bool DEBUG = false;

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

    if (DEBUG) {
        print_battlefield(team1, team2);
    }
	return((team1->getPlayer(0)->isAlive() || team1->getPlayer(1)->isAlive() || team1->getPlayer(2)->isAlive() || team1->getPlayer(3)->isAlive() || team1->getPlayer(4)->isAlive()) 
		&& (team2->getPlayer(0)->isAlive() || team2->getPlayer(1)->isAlive() || team2->getPlayer(2)->isAlive() || team2->getPlayer(3)->isAlive() || team2->getPlayer(4)->isAlive()));
}
/*
std::vector<double> winrate(std::vector<Team*> teams) {
    std::vector<int> win_count = std::vector<int>();
    for (size_t i = 0; i < teams.size(); i++) {
        win_count.push_back(0);
    }
    size_t vector_size = teams.size();
    for (size_t i = 0; i < vector_size; i++) {
        Team* team1 = teams[i];
        for (size_t j = i + 1; j < vector_size; j++) {
            Team* team2 = teams[j];
            FightResult result = bo3(team1, team2);
            if (result == TEAM1_WIN) {
                win_count[i]++;
            } else {
                win_count[j]++;
            }
        }
    }
    std::vector<double> win_rate = std::vector<double>();
    for (size_t i = 0; i < vector_size; i++) {
        double wr = static_cast<double>(win_count[i]) / static_cast<double>(vector_size - 1);
        win_rate.push_back(wr);
    }
    return win_rate;
}


std::vector<double> winrate_random_5(std::vector<Team*> teams, int fights_per_team = 5) {
    const size_t n = teams.size();
    std::vector<int> win_count(n, 0);
    std::vector<int> games_played(n, 0);

    if (n <= 1) {
        return std::vector<double>(n, 0.0);
    }

    const int k = std::min<int>(fights_per_team, static_cast<int>(n - 1));

    for (size_t i = 0; i < n; ++i) {
        std::unordered_set<size_t> chosen;
        chosen.reserve(static_cast<size_t>(k) * 2);

        while (static_cast<int>(chosen.size()) < k) {
            size_t j = static_cast<size_t>(rng::randint(0, static_cast<int>(n - 1)));
            if (j == i) continue;
            if (!chosen.insert(j).second) continue; // ya elegido

            Team* team1 = teams[i];
            Team* team2 = teams[j];

            FightResult result = bo3(team1, team2);

            // TEAM1 == team1 (teams[i])
            if (result == TEAM1_WIN) {
                win_count[i]++;
            } else {
                win_count[j]++;
            }

            games_played[i]++;
            games_played[j]++;
        }
    }

    std::vector<double> win_rate;
    win_rate.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        double wr = (games_played[i] == 0)
            ? 0.0
            : static_cast<double>(win_count[i]) / static_cast<double>(games_played[i]);
        win_rate.push_back(wr);
    }
    return win_rate;
}




///


// Igual que winrate(...), pero además cada team juega contra TODOS los anchors.
std::vector<double> winrate_anchor(const std::vector<Team*>& teams,
                                   const std::vector<Team*>& anchors) {
    const size_t n = teams.size();
    const size_t a = anchors.size();

    std::vector<int> win_count(n, 0);

    // 1) Round-robin dentro de la población (igual que tu winrate)
    for (size_t i = 0; i < n; i++) {
        Team* team1 = teams[i];
        for (size_t j = i + 1; j < n; j++) {
            Team* team2 = teams[j];
            FightResult result = bo3(team1, team2);
            if (result == TEAM1_WIN) win_count[i]++;
            else                     win_count[j]++;
        }
    }

    // 2) Cada team vs todos los anchors
    for (size_t i = 0; i < n; i++) {
        Team* team = teams[i];
        for (size_t k = 0; k < a; k++) {
            FightResult result = bo3(team, anchors[k]); // TEAM1 == team
            if (result == TEAM1_WIN) win_count[i]++;
        }
    }

    // 3) Winrate: total de games jugados por team = (n-1) + a
    std::vector<double> win_rate;
    win_rate.reserve(n);

    const double denom = (n == 0) ? 0.0 : (double)((n > 0 ? (n - 1) : 0) + a);

    for (size_t i = 0; i < n; i++) {
        double wr = (denom == 0.0) ? 0.0 : (double)win_count[i] / denom;
        win_rate.push_back(wr);
    }

    return win_rate;
}

*/
// Igual que winrate_random_5(...), pero además cada team juega contra TODOS los anchors.
// TODO: parallelize
std::vector<double> winrate_anchor_random_k(const std::vector<Team*>& teams, const std::vector<Team*>& anchors, const std::vector<Team*>& hof, int fights_per_team)
{
    const int n = (int)teams.size();
    const int a = (int)anchors.size();

    if (n == 0) return {};
    if (n == 1 && a == 0) return {0.0};

    const int k = (n <= 1) ? 0 : std::min(fights_per_team, n - 1);

    std::vector<int> win_count(n, 0);
    std::vector<int> games_played(n, 0);

    // TLS por thead
	omp_set_num_threads(16);
    const int T = omp_get_max_threads();
    std::vector<std::vector<int>> win_tls(T, std::vector<int>(n, 0));
    std::vector<std::vector<int>> games_tls(T, std::vector<int>(n, 0));

    // population vs k random
    #pragma omp parallel
    {
        const int tid = omp_get_thread_num();
        auto& win_local   = win_tls[tid];
        auto& games_local = games_tls[tid];

        std::vector<char> used(n); // bitset local reutilizable

        #pragma omp for schedule(static)
        for (int i = 0; i < n; ++i) {

            std::fill(used.begin(), used.end(), 0);

            int picked = 0;
            int draw   = 0;

            while (picked < k) {

                uint64_t ctx = rng_ctx(
                    GLOBAL_SEED,
                    (uint64_t)i,
                    (uint64_t)draw++,
                    rng_tag::MATCHUP_RAND
                );

                int j = rng::randint(ctx, 0, n - 1);

                if (j == i || used[j]) continue;
                used[j] = 1;
                picked++;

                FightResult r = bo3_copy(teams[i], teams[j]);

                if (r == TEAM1_WIN) win_local[i]++;
                else                win_local[j]++;

                games_local[i]++;
                games_local[j]++;
            }
        }
    }

    // population vs anchors
    #pragma omp parallel
    {
        const int tid = omp_get_thread_num();
        auto& win_local   = win_tls[tid];
        auto& games_local = games_tls[tid];

        #pragma omp for schedule(static)
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < a; ++j) {
                FightResult r = bo3_copy(teams[i], anchors[j]);
                if (r == TEAM1_WIN) win_local[i]++;
                games_local[i]++;
            }
        }
    }

	// population vs HoF
	#pragma omp parallel
	{
		const int tid = omp_get_thread_num();
		auto& win_local   = win_tls[tid];
		auto& games_local = games_tls[tid];

		#pragma omp for schedule(static)
		for (int i = 0; i < n; ++i) {
			for (int h = 0; h < hof.size(); ++h) {
				FightResult r = bo3_copy(teams[i], hof[h]);

				if (r == TEAM1_WIN)
					win_local[i]++;

				games_local[i]++;
			}
		}
	}



    // final reduction
    for (int t = 0; t < T; ++t) {
        for (int i = 0; i < n; ++i) {
            win_count[i]    += win_tls[t][i];
            games_played[i] += games_tls[t][i];
        }
    }

    // winrate
    std::vector<double> win_rate(n, 0.0);
    for (int i = 0; i < n; ++i) {
        if (games_played[i] > 0) {
            win_rate[i] =
                (double)win_count[i] / (double)games_played[i];
        }
    }

    return win_rate;
}






FightResult bo3(Team* team1, Team* team2) {
	int wins_team_1 = 0;
	int wins_team_2 = 0;
	
    //first match
	if (fight(team1, team2) == TEAM1_WIN)
		wins_team_1++;
	else
		wins_team_2++;
	
    //second match
	if (fight(team1, team2) == TEAM1_WIN)
		wins_team_1++;
	else
		wins_team_2++;
	
    if (wins_team_1 == 2) 
		return TEAM1_WIN;
	if (wins_team_2 == 2)
		return TEAM2_WIN;
	
	//final match
	return (fight(team1, team2));
}

void fire_circle(Team* team1, Team* team2, float true_dmg) {
    for (int i = 0; i < 5; ++i) {
        Player* p_t1 = team1->getPlayer(i);
        Player* p_t2 = team2->getPlayer(i);
        if (p_t1->isAlive()) {
            p_t1->getDynStats()->hp -= (int)roundf(getStatMaxHp(p_t1->getStatPoints()->max_hp) * true_dmg);
            if (p_t1->getDynStats()->hp < 0) {
                p_t1->setAlive(false);
                p_t1->getDynStats()->hp = 0;
            }
        }
        if (p_t2->isAlive()) {
            p_t2->getDynStats()->hp -= (int)roundf(getStatMaxHp(p_t2->getStatPoints()->max_hp) * true_dmg);
            if (p_t2->getDynStats()->hp < 0) {
                p_t2->setAlive(false);
                p_t2->getDynStats()->hp = 0;
            }
        }
    }
}

FightResult fight(Team* team1, Team* team2) {
    bool end = false;
    int it = 0;
    for (int i = 0; i < 5; i++) {
        team1->getPlayer(i)->_init_player();
        team2->getPlayer(i)->_init_player();
    }
    float fire_circle_dmg = 0.0f;
    do {
        end = !step(team1, team2);
        if (++it % 20000 == 0) {
            fire_circle_dmg = fire_circle_dmg + 0.05f;
        }
        if (it % 100 == 0) {
            fire_circle(team1, team2, fire_circle_dmg); // 5% max HP true damage every 20k iterations
        }
        bool team1_alive = team1->getPlayer(0)->isAlive() || team1->getPlayer(1)->isAlive() || team1->getPlayer(2)->isAlive() || team1->getPlayer(3)->isAlive() || team1->getPlayer(4)->isAlive();
        bool team2_alive = team2->getPlayer(0)->isAlive() || team2->getPlayer(1)->isAlive() || team2->getPlayer(2)->isAlive() || team2->getPlayer(3)->isAlive() || team2->getPlayer(4)->isAlive();
        if (end) {
            if (team1_alive && !team2_alive)
                return TEAM1_WIN;
            else 
				return TEAM2_WIN;
        }
        if (DEBUG) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    } while (!end);
    std::cout << "Fight ended in " << it << " iterations.\n";
    return TEAM1_WIN;
}

int64_t runLoop(int64_t n) {
    int64_t sum = 0;
    for (int64_t i = 0; i < n; i++) sum += i;
    return sum;
}

void chooseInstance(Instance i) {
	instance = i;
}

FightResult bo3_copy(const Team* team1, const Team* team2) {
    Team t1 = team1->clone();
    Team t2 = team2->clone();
    return bo3(&t1, &t2);
}


