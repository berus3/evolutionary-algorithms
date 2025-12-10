#include "../include/main.hpp"

int main() {
	Instance i = BALANCED;
	int inp;
	std::cin >> inp; 
	float a = getStatSmite(inp, i);
	std::cout << a;
	return 0;
}
