// rng.cpp
#include "rng.hpp"

namespace rng {

	static inline uint64_t splitmix64(uint64_t x) {
		x += 0x9e3779b97f4a7c15ULL;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
		x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
		return x ^ (x >> 31);
	}

	uint64_t mix(uint64_t x) {
		return splitmix64(x);
	}

	double real01(uint64_t seed) {
		return (mix(seed) >> 11) * (1.0 / (1ULL << 53));
	}

	int randint(uint64_t seed, int a, int b) {
		return a + (mix(seed) % (uint64_t)(b - a + 1));
	}

}
