#include "../include/main.hpp"
#include <chrono>
#include <algorithm>
#include <numeric>
#include <iostream>

// helpers para imprimir
static double mean(const std::vector<double>& v) {
    if (v.empty()) return 0.0;
    return std::accumulate(v.begin(), v.end(), 0.0) / static_cast<double>(v.size());
}

static std::vector<size_t> topk_indices(const std::vector<double>& v, size_t k) {
    std::vector<size_t> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);
    if (k > idx.size()) k = idx.size();
    std::partial_sort(
        idx.begin(), idx.begin() + k, idx.end(),
        [&](size_t a, size_t b) { return v[a] > v[b]; }
    );
    idx.resize(k);
    return idx;
}

int main() {
    chooseInstance(BALANCED);
/*
    // Crear teams (100) con 5 players cada uno
    const int N = 100;
    std::vector<Team*> teams;
    teams.reserve(N);

    for (int i = 0; i < N; ++i) {
        Team* t = new Team(i);
        for (int j = 0; j < 5; ++j) {
            t->setPlayer(j, new Player(j + i * 5)); // ids únicos (opcional)
        }
        // si tu lógica requiere init explícito:
        for (int j = 0; j < 5; ++j) {
            t->getPlayer(j)->_init_player();
        }
        teams.push_back(t);
    }

    // 1) Winrate ALL-vs-ALL
    const auto t0 = std::chrono::steady_clock::now();
    std::vector<double> wr_full = winrate(teams);
    const auto t1 = std::chrono::steady_clock::now();
    const auto ms_full = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();



    // 2) Winrate 5 random por team
    const auto t2 = std::chrono::steady_clock::now();
    std::vector<double> wr_r5 = winrate_random_5(teams, 1); // o winrate_random_5(teams)
    const auto t3 = std::chrono::steady_clock::now();
    const auto ms_r5 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();

    std::cout << "\n=== Timing ===\n";
    std::cout << "Full round-robin: " << ms_full << " ms\n";
    std::cout << "5-random per team: " << ms_r5 << " ms\n";
    if (ms_r5 > 0) {
        std::cout << "Speedup ~ " << (static_cast<double>(ms_full) / static_cast<double>(ms_r5)) << "x\n";
    }

    // Resumen para comparar distribución (no van a coincidir exactamente)
    std::cout << "\n=== Sanity check ===\n";
    std::cout << "Mean WR full: " << mean(wr_full) << "\n";
    std::cout << "Mean WR r5  : " << mean(wr_r5) << "\n";

    auto top_full = topk_indices(wr_full, 5);
    auto top_r5   = topk_indices(wr_r5, 5);

    std::cout << "\nTop-5 full:\n";
    for (auto i : top_full) {
        std::cout << "  Team " << i << " -> " << (wr_full[i] * 100.0) << "%\n";
    }

    std::cout << "\nTop-5 r5:\n";
    for (auto i : top_r5) {
        std::cout << "  Team " << i << " -> " << (wr_r5[i] * 100.0) << "%\n";
    }

    // Cleanup
    for (Team* t : teams) delete t;
*/
    return 0;
}
