#include "../include/main.hpp"

int main() {
	chooseInstance(BALANCED);
	int inp;
	std::cin >> inp; 
	float a = getStatCrit(inp);
	std::cout << a;
	return 0;
}
