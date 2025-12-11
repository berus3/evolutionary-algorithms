#include "../include/main.hpp"

int main() {
    chooseInstance(BALANCED);

		Team* t1 = new Team(1);
		Team* t2 = new Team(2);
		for (int i = 0; i < 5; i++) {
			t1->setPlayer(i, new Player(i));
			t2->setPlayer(i, new Player(i+5));
		}
		/*
		std::cout << "ap " << getStatAp(t1->getPlayer(0)->getStatPoints()->ap) << "\n";
		std::cout << "ax "<< getStatAx(t1->getPlayer(0)->getStatPoints()->ax) << "\n";
		std::cout << "acc "<< getStatAcc(t1->getPlayer(0)->getStatPoints()->acc) << "\n";
		std::cout << "acc ticks" << getStatAccTicks(t1->getPlayer(0)->getStatPoints()->acc_ticks) << "\n";
		std::cout << "cd acc " << getStatCdAcc(t1->getPlayer(0)->getStatPoints()->cd_acc) << "\n";
		std::cout << "ah " << getStatAh(t1->getPlayer(0)->getStatPoints()->ah) << "\n";
		Player* target = t1->getPlayer(0);
		int ticks_duracion_acc = (getStatAp(target->getStatPoints()->ap) + getStatAx(target->getStatPoints()->ax)) * getStatAccTicks(target->getStatPoints()->acc_ticks); // (ap + ax) * acc ticks
		int reduccion_as = getStatAs(target->getStatPoints()->as) * getStatAcc(target->getStatPoints()->acc); // ally_as * acc
		int reduccion_ah = getStatAh(target->getStatPoints()->ah) * getStatAcc(target->getStatPoints()->acc); // ally_ah * acc
		int cooldown = target->_haste(getStatCdAcc(target->getStatPoints()->cd_acc));
		std::cout << "el acc dura " << ticks_duracion_acc << " ticks y se tiene cada " << cooldown  << " ticks" << "\n";
		*/
		
		//Player 0: adc
		t1->getPlayer(0)->getStatPoints()->ad = 25;
		t1->getPlayer(0)->getStatPoints()->as = 25;
		
		
		//Player 1: tanque
		
		//Player 2: supp peel
		
		//Player 3: bruiser
		
		//Player 4: mago
		
		t2->getPlayer(0)->getStatPoints()->bleed_dmg = 15;
		t2->getPlayer(0)->getStatPoints()->bleed_ticks = 15;
		t2->getPlayer(0)->getStatPoints()->bleed = 20;
		
		fight(t1, t2);
		
		
		delete t1;
		delete t2;

    return 0;
}
