#include "../include/main.hpp"

int main() {
    chooseInstance(BALANCED);

		Team* t1 = new Team(1);
		Team* t2 = new Team(2);
		for (int i = 0; i < 5; i++) {
			t1->setPlayer(i, new Player(i));
			t2->setPlayer(i, new Player(i+5));
		}
		std::cout << getStatAp(t1->getPlayer(0)->getStatPoints()->ap) << "\n";
		std::cout << getStatAx(t1->getPlayer(0)->getStatPoints()->ax) << "\n";
		std::cout << getStatAcc(t1->getPlayer(0)->getStatPoints()->acc) << "\n";
		std::cout << getStatAccTicks(t1->getPlayer(0)->getStatPoints()->acc_ticks) << "\n";
		std::cout << getStatCdAcc(t1->getPlayer(0)->getStatPoints()->cd_acc) << "\n";
		//t1->getPlayer(0)->getStatPoints()->stun = 100;
		//t1->getPlayer(0)->getStatPoints()->cd_stun = 100;
		
		fight(t1, t2);
		delete t1;
		delete t2;

    return 0;
}
