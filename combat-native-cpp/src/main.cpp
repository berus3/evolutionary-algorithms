#include "../include/main.hpp"

int main() {
	Instance i = BALANCED;
	int inp;
	std::cin >> inp; 
	int a = getStatCdBlast(inp, i);
	std::cout << a;
	return 0;
}
