#include "../include/player.hpp"

Player::Player(int id) { 
    this->player_id = id; 
    this->is_alive = true;
}
Player::~Player() {}
int Player::getId() { return player_id; }
StatPoints* Player::getStatPoints() { return stat_points; }
void Player::setStatPoints(StatPoints* sp) { stat_points = sp; }
void Player::act(Team* allies, Team* enemies) {
    if (is_alive) {
        update_effects();
        regen();
        attack(enemies);
       
        // smite
        // blast
        // heal
        // stun
        // acc
        // slow
        // shield
        // mark
    } else {
        dyn_stats->hp = 0;
    }
}

void Player::heal(Player* target, int healing) {
	int maxHp = getStatMaxHp(target->getStatPoints->max_hp);
	target->getDynStats()->hp += healing;
	if (target->getDynStats()->hp > maxHp) {
		target->getDynStats()->hp = maxHp;
	}
}

void Player::regen(){
	if (dyn_stats->next_regen == 0) {
		dyn_stats->next_regen = 50;
		heal(this, getStatRegen(stat_points->regen));
	} else {
		dyn_stats->next_regen--;
	}
}

int Player::apply_crit(int damage) {
	if (getStatCrit(stat_points->crit) > rng::real01) {
		damage = (int)roundf(damage * getStatCritFactor(stat_points->crit_factor));
	}
	return damage;
}

int Player::reduce_ad(Player* target, int damage_output) {
	int total_armor = getStatArmor(target->getStatPoints()->armor); //armor base
	total_armor = (int)roundf(total_armor * (1 - getStatArmorPen(stat_points->armor_pen))) - getStatLethality(stat_points->lethality); //aplicar armor-pen y lethality
	
	if (target->getDynStats()->end_shield > 0)
		total_armor += target->getDynStats()->shield_resistance;
	
	if (target->getDynStats()->end_mark > 0)
		total_armor -= target->getDynStats()->mark_resistance;
		
	float reduction = 1.0f - (100.0f / (100.0f + total_armor)); //(1-(100/(100+armor)))
	return (int)roundf(damage_output * reduction);
}

int Player::reduce_ap(Player* target, int damage_output) {
	int total_mr = getStatMr(target->getStatPoints()->mr); //mr base
	total_mr = (int)roundf(total_mr * (1 - getStatMrPen(stat_points->mr_pen))) - getStatEthereal(stat_points->ethereal); //aplicar mr-pen y ethereal
	
	if (target->getDynStats()->end_shield > 0)
		total_mr += target->getDynStats()->shield_resistance;
	
	if (target->getDynStats()->end_mark > 0)
		total_mr -= target->getDynStats()->mark_resistance;
		
	float reduction = 1.0f - (100.0f / (100.0f + total_mr)); //(1-(100/(100+armor)))
	return (int)roundf(damage_output * reduction);
}

void Player::damage_ap(Player* target, int damage_output) {
	int damage_dealt = reduce_ap(damage_output);
	
	if (target->getDynStats->hp <= 0)
		target->getDynStats->is_alive = false;
}

void Player::damage_ad(Player* target, int damage_output) {
	int damage_dealt = reduce_ad(damage_output);
	
	if (target->getDynStats->hp <= 0)
		target->getDynStats->is_alive = false;
	
	if (vamp_healing > 0) {
		heal((int)roundf(damage_dealt * getStatVamp(stat_points->vamp)));
	}
	
	float target_thorns = target->getStatThorns(stat_points->thorns);
	if (target_thorns > 0) {
		damage_ap(this, (int)roundf(damage_dealt * target_thorns)); //TODO: verificar lógica
	}
}


void Player::update_effects() {
	if (dyn_stats->end_acc == 0)
		dyn_stats->acc_ticks = 0;
	else
		dyn_stats->end_acc --;
	
	if (dyn_stats->end_slow == 0)
		dyn_stats->slow_ticks = 0;
	else
		dyn_stats->end_slow --;
	//TODO mark y shield
	//TODO seguir
}

void Player::bleed(Player* player) {
	
	
}

void Player::attack(Team* enemies) {
	//int acc_ticks = (int)roundf((dyn_stats->buff_acc * getStatAs(stat_points->as));
	//int slow_ticks = (int)roundf((dyn_stats->nerf_slow * getStatAs(stat_points->as));
	
	if ((dyn_stats->next_attack - dyn_stats->acc_ticks + dyn_stats->slow_ticks) <= 0) {
		dyn_stats->next_attack = getStatAs(stat_points->as);
		Player* target = selectAttackTarget(enemies);
		int damage_output = apply_crit(getStatAd(stat_points->ad) + getStatAx(stat_points->ax)
		damage_ad(target, damage_output);	
		if ()	
	} else
		dyn_stats->next_attack--;
}

Player* Player::selectAttackTarget(Team* enemies) {
	return nullptr;
}
