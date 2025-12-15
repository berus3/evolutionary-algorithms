#ifndef RNG_HPP
#define RNG_HPP
#pragma once
#include <cstdint>

namespace rng {

	uint64_t mix(uint64_t x);

	// Random in [0,1)
	double real01(uint64_t seed);

	// Integer in [a,b]
	int randint(uint64_t seed, int a, int b);

}

#endif // RNG_HPP