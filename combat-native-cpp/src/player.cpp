#include "../include/player.hpp"
#include "../include/team.hpp"

Player::Player(int id) { 
    this->_player_id = id; 
    this->_is_alive = true;
}
Player::~Player() {}
int Player::getId() { return _player_id; }
StatPoints* Player::getStatPoints() { return _stat_points; }
DynamicStats* Player::getDynStats() { return _dyn_stats; }
void Player::setStatPoints(StatPoints* sp) { _stat_points = sp; }
void Player::act(Team* allies, Team* enemies) {
    if (_is_alive) {
        _update_effects();
        _regen();
        _attack(enemies);
        _receive_bleed();
        
        // smite
        // blast
        // heal
        // stun
        // acc
        // slow
        // shield
        // mark
    } else {
        _dyn_stats->hp = 0;
    }
}

void Player::_heal(Player* target, int healing) {
	int maxHp = getStatMaxHp(target->getStatPoints()->max_hp);
	target->getDynStats()->hp += healing;
	if (target->getDynStats()->hp > maxHp) {
		target->getDynStats()->hp = maxHp;
	}
}

void Player::_regen() {
	if (_dyn_stats->next_regen == 0) {
		_dyn_stats->next_regen = 50;
		_heal(this, getStatRegen(_stat_points->regen));
	} else {
		_dyn_stats->next_regen--;
	}
}

void Player::_receive_bleed() {
	if (_dyn_stats->next_bleed == 0) {
		_dyn_stats->next_bleed = 100;
		_damage_bleed(this, _dyn_stats->bleed_accumulated_damage);
	}
}

int Player::_apply_crit(int damage) {
	if (getStatCrit(_stat_points->crit) > rng::real01()) {
		damage = (int)roundf(damage * getStatCritFactor(_stat_points->crit_factor));
	}
	return damage;
}

int Player::_reduce_ad(Player* target, int damage_output) {
	int total_armor = getStatArmor(target->getStatPoints()->armor); //armor base
	total_armor = (int)roundf(total_armor * (1 - getStatArmorPen(_stat_points->armor_pen))) - getStatLethality(_stat_points->lethality); //aplicar armor-pen y lethality
	
	if (target->getDynStats()->end_shield > 0)
		total_armor += target->getDynStats()->shield_resistance;
	
	if (target->getDynStats()->end_mark > 0)
		total_armor -= target->getDynStats()->mark_resistance;
		
	float reduction = 1.0f - (100.0f / (100.0f + total_armor)); //(1-(100/(100+armor)))
	return (int)roundf(damage_output * reduction);
}

int Player::_reduce_ap(Player* target, int damage_output) {
	int total_mr = getStatMr(target->getStatPoints()->mr); //mr base
	total_mr = (int)roundf(total_mr * (1 - getStatMrPen(_stat_points->mr_pen))) - getStatEthereal(_stat_points->ethereal); //aplicar mr-pen y ethereal
	
	if (target->getDynStats()->end_shield > 0)
		total_mr += target->getDynStats()->shield_resistance;
	
	if (target->getDynStats()->end_mark > 0)
		total_mr -= target->getDynStats()->mark_resistance;
		
	float reduction = 1.0f - (100.0f / (100.0f + total_mr)); //(1-(100/(100+armor)))
	return (int)roundf(damage_output * reduction);
}

void Player::_damage_ap(Player* target, int damage_output) {
	int damage_dealt = _reduce_ap(target, damage_output);
	target->getDynStats()->hp -= damage_dealt;

	if (target->getDynStats()->hp <= 0)
		target->_is_alive = false;
}

void Player::_damage_ad(Player* target, int damage_output) {
	int damage_dealt = _reduce_ad(target, damage_output);
	target->getDynStats()->hp -= damage_dealt;

	if (target->getDynStats()->hp <= 0)
		target->_is_alive = false;
	
	_heal(this, (int)roundf(getStatVamp(_stat_points->vamp) * damage_dealt));
	_bleed(target, damage_dealt); 
	
	float target_thorns = getStatThorns(target->getStatPoints()->thorns);
	if (target_thorns > 0) {
		_damage_ap(this, (int)roundf(damage_dealt * target_thorns)); //TODO: verificar lógica
	}
}

void Player::_update_effects() {
	if (_dyn_stats->end_acc == 0)
		_dyn_stats->acc_ticks = 0;
	else
		_dyn_stats->end_acc --;
	
	if (_dyn_stats->end_slow == 0)
		_dyn_stats->slow_ticks = 0;
	else
		_dyn_stats->end_slow --;
		
	if (_dyn_stats->end_mark == 0)
		_dyn_stats->mark_resistance = 0;
	else 
		_dyn_stats->mark_resistance--;
		
	if (_dyn_stats->end_shield == 0)
		_dyn_stats->shield_resistance = 0;
	else 
		_dyn_stats->shield_resistance--;
		
	if (_dyn_stats->end_bleed == 0) {
		_dyn_stats->bleed_accumulated_damage = 0;
		_dyn_stats->bleed_stacks = 0;
	} else 
		_dyn_stats->end_bleed--;
		
	//TODO seguir
}

void Player::_bleed(Player* target, int damage_dealt) {
	 if (getStatBleed(_stat_points->bleed) > rng::real01()) {		
		 if (target->getDynStats()->bleed_stacks < 10) {
			target->getDynStats()->bleed_stacks ++;
			target->getDynStats()->bleed_accumulated_damage += (int)roundf((getStatBleedDmg(_stat_points->bleed_dmg) * (getStatAd(_stat_points->ad) + getStatAx(_stat_points->ax))));
		 }
		 target->getDynStats()->end_bleed = getStatBleedTicks(_stat_points->bleed_ticks);
	 }
}

void Player::_damage_bleed(Player* target, int damage_output) {
	int damage_dealt = _reduce_ad(target, damage_output);
	
	if (target->getDynStats()->hp <= 0)
		target->_is_alive = false;
}



void Player::_attack(Team* enemies) {
	if ((_dyn_stats->next_attack - _dyn_stats->acc_ticks + _dyn_stats->slow_ticks) <= 0) {
		_dyn_stats->next_attack = getStatAs(_stat_points->as);
		Player* target = _select_attack_target(enemies);
		int damage_output = _apply_crit(getStatAd(_stat_points->ad) + getStatAx(_stat_points->ax));
		_damage_ad(target, damage_output);	
	} else
		_dyn_stats->next_attack--;
}

Player* Player::_select_attack_target(Team* enemies) {
	int denom = 0;
	int reduced_hp[5];
	for (int i=0; i<5; i++) {
		Player* target = enemies->getPlayer(i);
		int remaining_hp = target->getDynStats()->hp;
		reduced_hp[i] = _reduce_ad(target, remaining_hp);
		denom += _reduce_ad(target, remaining_hp);
	}
	float weights[5];
	for (int i=0; i<5; i++) {
		Player* target = enemies->getPlayer(i);
		weights[i] = getStatAggro(target->getStatPoints()->aggro) + (getStatFocus(_stat_points->focus) * (1-(reduced_hp[i]/denom)));
	}
	return enemies->getPlayer(linear_softmax(weights, 5));
}
