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

void Player::heal(int healing) {
	int maxHp = getStatMaxHp(stat_points->max_hp);
	dyn_stats->hp += healing;
	if (dyn_stats->hp > maxHp) {
		dyn_stats->hp = maxHp;
	}
}

void Player::regen(){
	if (dyn_stats->next_regen == 0) {
		dyn_stats->next_regen = 50;
		heal(getStatRegen(stat_points->regen));
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

int Player::reduce(Player* target, int damage_output) {
	int total_armor = getStatArmor(target->getStatPoints()->armor); //armor base
	int total_armor = getStatArmor(target->getStatPoints()->armor); //armor sin mark
	total_armor = (int)roundf(total_armor * (1 - getStatArmorPen(stat_points->armor_pen))) - getStatLethality(stat_points->lethality); //armor con lethality y 
	float reduction = 1.0f - (100.0f / (100.0f + total_armor)) //(1-(100/(100+armor)))
	int damage_input = (int)roundf(damage_output * reduction);
}

void Player::damage_ad(Player* player) {
	return;
	int damage_output = apply_crit(getStatAd(stat_points->ad) + getStatAx(stat_points->ax));
	
	int damage_dealt = 0;
	//aplicarle dano segun armor 
	//si murio, marcarlo como muerto
	
	//vamp
	int vamp_healing = (int)roundf(damage_dealt * getStatVamp(stat_points->vamp));
		if (vamp_healing > 0)
			heal(vamp_healing);
	
}

void Player::damage_ap(Player* player) {
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
		damage_ad(target);		
	} else
		dyn_stats->next_attack--;
}

Player* Player::selectAttackTarget(Team* enemies) {
	return nullptr;
}
