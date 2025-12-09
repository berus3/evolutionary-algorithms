#include "../include/main.hpp"

int main() {
	Instance i = BALANCED;
	int inp;
	std::cin >> inp; 
	int a = getStatBleedTicks(inp, i);
	std::cout << a;
	return 0;
}
