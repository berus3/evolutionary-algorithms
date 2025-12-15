#ifndef RNG_CONTEXT_HPP
#define RNG_CONTEXT_HPP
#pragma once
#include <cstdint>

// ============================================================
// Global seed (it comes from JNI)
// ============================================================
extern uint64_t GLOBAL_SEED;

// ============================================================
// RNG semantic tags (one by event)
// ============================================================
enum class rng_tag : uint64_t {
    CRIT = 1,
    BLEED,

    SOFTMAX_ATK,
    SOFTMAX_SLOW,
    SOFTMAX_ACC,
    SOFTMAX_SMITE,
    SOFTMAX_HEAL,
    SOFTMAX_STUN,
    SOFTMAX_SHIELD,
    SOFTMAX_MARK,

    STAT_ROLL,
    MATCHUP_RAND
};

// ============================================================
// RNG context builder (deterministic and parallel)
// ============================================================
inline uint64_t rng_ctx(
    uint64_t global_seed,
    uint64_t a,
    uint64_t b,
    rng_tag tag
) {
    return global_seed
         ^ (a * 0x9E3779B97F4A7C15ULL)
         ^ (b * 0xBF58476D1CE4E5B9ULL)
         ^ static_cast<uint64_t>(tag);
}

#endif // RNG_CONTEXT_HPP