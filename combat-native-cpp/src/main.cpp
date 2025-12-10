#include "../include/main.hpp"

int main() {
	chooseInstance(BALANCED);
	std::cout << rng::real01() << std::endl;
	int bla;
	std::cin >> bla;
	std::cout << getStatCrit(bla) << std::endl;
	return 0;
}
