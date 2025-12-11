#include "../include/main.hpp"

int main() {
    chooseInstance(BALANCED);

		Team* t1 = new Team(1);
		Team* t2 = new Team(2);
		for (int i = 0; i < 5; i++) {
			t1->setPlayer(i, new Player(i));
			t2->setPlayer(i, new Player(i+5));
		}
		//t1->getPlayer(0)->getStatPoints()->bleed = 100;
		
		fight(t1, t2);
		delete t1;
		delete t2;

    return 0;
}
