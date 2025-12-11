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
		t1->getPlayer(0)->getStatPoints()->as = 20;
		t1->getPlayer(0)->getStatPoints()->crit = 20;
		t1->getPlayer(0)->getStatPoints()->crit_factor = 15;
		t1->getPlayer(0)->getStatPoints()->focus = 20;	
		
		//Player 1: tanque
		t1->getPlayer(1)->getStatPoints()->max_hp = 20;
		t1->getPlayer(1)->getStatPoints()->thorns = 20;
		t1->getPlayer(1)->getStatPoints()->aggro = 20;
		t1->getPlayer(1)->getStatPoints()->armor = 20;
		t1->getPlayer(1)->getStatPoints()->mr = 20;
		
		//Player 2: supp peel
		t1->getPlayer(2)->getStatPoints()->ap = 30;
		t1->getPlayer(2)->getStatPoints()->heal = 30;
		t1->getPlayer(2)->getStatPoints()->cd_heal = 20;
		t1->getPlayer(2)->getStatPoints()->focus = 10;
		t1->getPlayer(2)->getStatPoints()->shield = 5;
		t1->getPlayer(2)->getStatPoints()->cd_shield = 5;
		
		//Player 3: bruiser
		t1->getPlayer(3)->getStatPoints()->ax = 30;
		t1->getPlayer(3)->getStatPoints()->vamp = 20;
		t1->getPlayer(3)->getStatPoints()->mark = 20;
		t1->getPlayer(3)->getStatPoints()->cd_mark = 20;
		t1->getPlayer(3)->getStatPoints()->mark_ticks = 10;
		
		//Player 4: mago
		t1->getPlayer(4)->getStatPoints()->ap = 40;
		t1->getPlayer(4)->getStatPoints()->blast = 40;
		t1->getPlayer(4)->getStatPoints()->cd_blast = 20;
		
		
		//Player 0: adc
        t2->getPlayer(0)->getStatPoints()->ad = 20;
        t2->getPlayer(0)->getStatPoints()->as = 20;
        t2->getPlayer(0)->getStatPoints()->crit = 20;
        t2->getPlayer(0)->getStatPoints()->crit_factor = 20;
        t2->getPlayer(0)->getStatPoints()->focus = 20;


        //Player 1: tanque
        t2->getPlayer(1)->getStatPoints()->max_hp = 40;
        t2->getPlayer(1)->getStatPoints()->aggro = 60;

        //Player 2: supp peel
        t2->getPlayer(2)->getStatPoints()->focus = 20;
        t2->getPlayer(2)->getStatPoints()->ah = 20;
        t2->getPlayer(2)->getStatPoints()->ap = 20;
        t2->getPlayer(2)->getStatPoints()->heal = 20;
        t2->getPlayer(2)->getStatPoints()->acc = 20;

        //Player 3: bruiser
        t2->getPlayer(3)->getStatPoints()->ad = 15;
        t2->getPlayer(3)->getStatPoints()->max_hp = 20;
        t2->getPlayer(3)->getStatPoints()->vamp = 50;
        t2->getPlayer(3)->getStatPoints()->aggro = 10;
        t2->getPlayer(3)->getStatPoints()->armor_pen = 5;

        //Player 4: mago
        t2->getPlayer(4)->getStatPoints()->ap = 30;
        t2->getPlayer(4)->getStatPoints()->smite = 20;
        t2->getPlayer(4)->getStatPoints()->focus = 10;
        t2->getPlayer(4)->getStatPoints()->ah = 20;
        t2->getPlayer(4)->getStatPoints()->mark = 20;

		
		for (int i = 0; i < 5; i++) {
			t1->getPlayer(i)->_init_player();
			t2->getPlayer(i)->_init_player();
		}
		
		fight(t1, t2);
		
		
		delete t1;
		delete t2;

    return 0;
}
