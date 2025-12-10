#include "../include/player.hpp"

Player::Player(int id) { 
    this->player_id = id; 
    this->is_alive = true;
}
Player::~Player() {}
int Player::getId() { return player_id; }
StatPoints* Player::getStatPoints() { return stat_points; }
DynamicStats* Player::getDynStats() { return dyn_stats; }
void Player::setStatPoints(StatPoints* sp) { stat_points = sp; }
void Player::act(Team* allies, Team* enemies) {
    if (is_alive) {
        update_effects();
        regen();
        attack(enemies);
        receive_bleed();
        
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
	int maxHp = getStatMaxHp(target->getStatPoints()->max_hp);
	target->getDynStats()->hp += healing;
	if (target->getDynStats()->hp > maxHp) {
		target->getDynStats()->hp = maxHp;
	}
}

void Player::regen() {
	if (dyn_stats->next_regen == 0) {
		dyn_stats->next_regen = 50;
		heal(this, getStatRegen(stat_points->regen));
	} else {
		dyn_stats->next_regen--;
	}
}

void Player::receive_bleed() {
	if (dyn_stats->next_bleed == 0) {
		dyn_stats->next_bleed = 100;
		damage_bleed(this, dyn_stats->bleed_accumulated_damage);
	}
}

int Player::apply_crit(int damage) {
	if (getStatCrit(stat_points->crit) > rng::real01()) {
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
	int damage_dealt = reduce_ap(target, damage_output);
	target->getDynStats()->hp -= damage_dealt;

	if (target->getDynStats()->hp <= 0)
		target->is_alive = false;
}

void Player::damage_ad(Player* target, int damage_output) {
	int damage_dealt = reduce_ad(target, damage_output);
	target->getDynStats()->hp -= damage_dealt;

	if (target->getDynStats()->hp <= 0)
		target->is_alive = false;
	
	heal(this, (int)roundf(getStatVamp(stat_points->vamp) * damage_dealt));
	bleed(target, damage_dealt); 
	
	float target_thorns = getStatThorns(target->getStatPoints()->thorns);
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
		
	if (dyn_stats->end_mark == 0)
		dyn_stats->mark_resistance = 0;
	else 
		dyn_stats->mark_resistance--;
		
	if (dyn_stats->end_shield == 0)
		dyn_stats->shield_resistance = 0;
	else 
		dyn_stats->shield_resistance--;
		
	if (dyn_stats->end_bleed == 0) {
		dyn_stats->bleed_accumulated_damage = 0;
		dyn_stats->bleed_stacks = 0;
	} else 
		dyn_stats->end_bleed--;
		
	//TODO seguir
}

void Player::bleed(Player* target, int damage_dealt) {
	 if (getStatBleed(stat_points->bleed) > rng::real01()) {		
		 if (target->getDynStats()->bleed_stacks < 10) {
			target->getDynStats()->bleed_stacks ++;
			target->getDynStats()->bleed_accumulated_damage += (int)roundf((getStatBleedDmg(stat_points->bleed_dmg) * (getStatAd(stat_points->ad) + getStatAx(stat_points->ax))));
		 }
		 target->getDynStats()->end_bleed = getStatBleedTicks(stat_points->bleed_ticks);
	 }
}

void Player::damage_bleed(Player* target, int damage_output) {
	int damage_dealt = reduce_ad(target, damage_output);
	
	if (target->getDynStats()->hp <= 0)
		target->is_alive = false;
}



void Player::attack(Team* enemies) {
	if ((dyn_stats->next_attack - dyn_stats->acc_ticks + dyn_stats->slow_ticks) <= 0) {
		dyn_stats->next_attack = getStatAs(stat_points->as);
		Player* target = selectAttackTarget(enemies);
		int damage_output = apply_crit(getStatAd(stat_points->ad) + getStatAx(stat_points->ax));
		damage_ad(target, damage_output);	
	} else
		dyn_stats->next_attack--;
}

Player* Player::selectAttackTarget(Team* enemies) {
	
	float weights[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	for (int i=0; i<5; i++) {
		
	}
	
	return nullptr;
}
