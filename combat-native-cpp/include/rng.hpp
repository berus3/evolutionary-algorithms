// rng.hpp
#pragma once
#include <cstdint>

namespace rng {

	uint64_t mix(uint64_t x);

	// Random en [0,1)
	double real01(uint64_t seed);

	// Entero [a,b]
	int randint(uint64_t seed, int a, int b);

}
