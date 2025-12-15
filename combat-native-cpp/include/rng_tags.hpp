#ifndef RNG_TAGS_HPP
#define RNG_TAGS_HPP
#pragma once
#include <cstdint>

namespace rng_tag {

    // Combat outcomes
    constexpr uint64_t CRIT          = 0x01;
    constexpr uint64_t BLEED_PROC    = 0x02;

    // Target selection
    constexpr uint64_t SOFTMAX_ATK   = 0x10;
    constexpr uint64_t SOFTMAX_HEAL  = 0x11;
    constexpr uint64_t SOFTMAX_ACC   = 0x12;
    constexpr uint64_t SOFTMAX_SLOW  = 0x13;
    constexpr uint64_t SOFTMAX_STUN  = 0x14;
    constexpr uint64_t SOFTMAX_MARK  = 0x15;
    constexpr uint64_t SOFTMAX_SHIELD= 0x16;
    constexpr uint64_t SOFTMAX_SMITE = 0x17;

    // Initialization / generation
    constexpr uint64_t STAT_ROLL     = 0x20;
    constexpr uint64_t MATCHUP_RAND  = 0x30;

}

#endif // RNG_TAGS_HPP