#include "../include/player.hpp"
#include "../include/team.hpp"
#include <iostream>

Player::Player(int id) {
    _player_id = id;
    this->_stat_points = new StatPoints();
    this->_dyn_stats = new DynamicStats();
    _randomize_stats();
    // _init_player();
}


Player::Player() { 
    this->_player_id = 0; 
    this->_is_alive = true;
    
    // _init_player();    
}

Player::~Player() {
	delete this->_stat_points;
	delete this->_dyn_stats;
}
int Player::getId() { return _player_id; }
StatPoints* Player::getStatPoints() { return _stat_points; }
DynamicStats* Player::getDynStats() { return _dyn_stats; }
void Player::setStatPoints(StatPoints* sp) { _stat_points = sp; }
void Player::act(Team* allies, Team* enemies) {
	_dyn_stats->track_healed_this_tick = false;
    if (_is_alive) {
        _update_effects();
        if (_dyn_stats->end_stun == 0) {
			_regen();
			_attack(enemies);
			if (_stat_points->ap > 0 || _stat_points->ax > 0) {
				if (_stat_points->acc > 0)	
					_apply_acc(allies);
				if (_stat_points->slow > 0)	
					_apply_slow(enemies);
				if (_stat_points->smite > 0)
					_smite(enemies);
				if (_stat_points->blast > 0)
					_blast(enemies);
				if (_stat_points->heal > 0)
					_heal(allies);
				if (_stat_points->stun > 0)
					_stun(enemies);
				if (_stat_points->shield > 0)
					_shield(allies);
				if (_stat_points->mark > 0)
					_mark(enemies);
			}		
		}
        _receive_bleed();
        
    } else {
        _dyn_stats->hp = 0;
        lastAttacked = -1;
    }
}

void Player::_raw_heal(Player* target, int healing) {
    int maxHp  = getStatMaxHp(target->getStatPoints()->max_hp);
    int before = target->getDynStats()->hp;
    target->getDynStats()->hp = std::min(before + healing, maxHp);
    int gained = target->getDynStats()->hp - before;
    if (gained > 0) {
        target->getDynStats()->track_hp_raw_healed += gained;
        target->getDynStats()->track_healed_this_tick = true; 
    }
}




void Player::_regen() {
	if (_dyn_stats->next_regen == 0) {
		_dyn_stats->next_regen = 50;
		_raw_heal(this, getStatRegen(_stat_points->regen));
	} else {
		_dyn_stats->next_regen--;
	}
}

void Player::_receive_bleed() {
	if (_dyn_stats->bleed_stacks <= 0)
        return;
	if (_dyn_stats->next_bleed == 0) {
		_dyn_stats->next_bleed = 100;
		_damage_bleed(this, _dyn_stats->bleed_accumulated_damage);
	} else
		_dyn_stats->next_bleed--;
}

int Player::_apply_crit(int damage) {
	if (getStatCrit(_stat_points->crit) > rng::real01()) {
		damage = (int)roundf(damage * getStatCritFactor(_stat_points->crit_factor));
	}
	return damage;
}

int Player::_reduce_ad(Player* target, int damage_output) {
	int total_armor = getStatArmor(target->getStatPoints()->armor); //armor base
	
	if (target->getDynStats()->end_shield > 0)
		total_armor += target->getDynStats()->shield_resistance;
	
	if (target->getDynStats()->end_mark > 0)
		total_armor -= target->getDynStats()->mark_resistance;
	
	total_armor = (int)roundf(total_armor * (1.0f - getStatArmorPen(_stat_points->armor_pen))) - getStatLethality(_stat_points->lethality); //aplicar armor-pen y lethality
		
	float reduction = 1.0f - (100.0f / (100.0f + total_armor)); //(1-(100/(100+armor)))
	return (int)roundf(damage_output * reduction);
}

int Player::_reduce_ap(Player* target, int damage_output) {
	int total_mr = getStatMr(target->getStatPoints()->mr); //mr base
	
	if (target->getDynStats()->end_shield > 0)
		total_mr += target->getDynStats()->shield_resistance;
	
	if (target->getDynStats()->end_mark > 0)
		total_mr -= target->getDynStats()->mark_resistance;
	
	total_mr = (int)roundf(total_mr * (1 - getStatMrPen(_stat_points->mr_pen))) - getStatEthereal(_stat_points->ethereal); //aplicar mr-pen y ethereal
		
	float reduction = 1.0f - (100.0f / (100.0f + total_mr)); //(1-(100/(100+armor)))
	return (int)roundf(damage_output * reduction);
}

void Player::_damage_ap(Player* target, int damage_output) {
	int damage_dealt = _reduce_ap(target, damage_output);
	target->getDynStats()->hp -= damage_dealt;

	target->getDynStats()->track_damage_received += damage_dealt;
	_dyn_stats->track_damage_dealt += damage_dealt;
	
	if (target->getDynStats()->hp <= 0) {
		target->getDynStats()->hp = 0;
		target->_is_alive = false;
	}
}

void Player::_damage_ad(Player* target, int damage_output) {
	int damage_dealt = _reduce_ad(target, damage_output);
	target->getDynStats()->hp -= damage_dealt;
	lastAttacked = target->getId();
	
	target->getDynStats()->track_damage_received += damage_dealt;
	_dyn_stats->track_damage_dealt += damage_dealt;
	
	if (target->getDynStats()->hp <= 0) {
		target->getDynStats()->hp = 0;
		target->_is_alive = false;
	}
	float vamp = getStatVamp(_stat_points->vamp);
	if (vamp > 0.0f)
		_raw_heal(this, (int)roundf(vamp * damage_dealt));
	
	if (_stat_points->bleed > 0)
		_bleed(target, damage_dealt);
	
	float target_thorns = getStatThorns(target->getStatPoints()->thorns);
	if (target_thorns > 0) {
		_damage_ap(this, (int)roundf(damage_dealt * target_thorns)); //TODO: verificar lógica
	}
}

void Player::_update_effects() {	
	//acc
	if (_dyn_stats->end_acc > 0) {
		_dyn_stats->end_acc--;
		if (_dyn_stats->end_acc == 0) {
			_dyn_stats->acc_as_ticks = 0;
			_dyn_stats->acc_ah_ticks = 0;
		}
	}

	//slow
	if (_dyn_stats->end_slow > 0) {
		_dyn_stats->end_slow--;
		if (_dyn_stats->end_slow == 0) {
			_dyn_stats->slow_as_ticks = 0;
			_dyn_stats->slow_ah_ticks = 0;
		}
	}

	//mark
	if (_dyn_stats->end_mark > 0) {
        _dyn_stats->end_mark--;
        if (_dyn_stats->end_mark == 0) {
            _dyn_stats->mark_resistance = 0;
        }
    }
		
	//shield
	if (_dyn_stats->end_shield > 0) {
        _dyn_stats->end_shield--;
        if (_dyn_stats->end_shield == 0) {
            _dyn_stats->shield_resistance = 0;
        }
    }
	
	//bleed
	if (_dyn_stats->end_bleed == 0) {
		_dyn_stats->bleed_accumulated_damage = 0;
		_dyn_stats->bleed_stacks = 0;
	} else 
		_dyn_stats->end_bleed--;
	
	//stun	
	if (_dyn_stats->end_stun > 0)
		_dyn_stats->end_stun--;
		
}

void Player::_bleed(Player* target, int damage_dealt) {
	if (getStatBleed(_stat_points->bleed) < rng::real01())
        return;
	 	
	 if (target->getDynStats()->bleed_stacks < 10) {
		target->getDynStats()->bleed_stacks ++;
		target->getDynStats()->bleed_accumulated_damage += (int)roundf((getStatBleedDmg(_stat_points->bleed_dmg) * damage_dealt));
		if (target->getDynStats()->bleed_stacks == 1) { //first stack
			target->getDynStats()-> next_bleed = 100;
		}
	 }
	 target->getDynStats()->end_bleed = getStatBleedTicks(_stat_points->bleed_ticks);
	 
}

void Player::_damage_bleed(Player* target, int damage_dealt) {
	target->getDynStats()->hp -= damage_dealt;
	
	target->getDynStats()->track_damage_received += damage_dealt;
	
	if (target->getDynStats()->hp <= 0) {
		target->getDynStats()->hp = 0;
		target->_is_alive = false;
	}
}



void Player::_attack(Team* enemies) {
	if ((_dyn_stats->next_attack - _dyn_stats->acc_as_ticks + _dyn_stats->slow_as_ticks) <= 0) {
		_dyn_stats->next_attack = getStatAs(_stat_points->as);
		Player* target = _select_attack_target(enemies);
		if (target == nullptr)
			return;
		int damage_output = _apply_crit(getStatAd(_stat_points->ad) + getStatAx(_stat_points->ax));
		_damage_ad(target, damage_output);	
	} else
		_dyn_stats->next_attack--;
}

int Player::_haste(int ticks) {
	return ticks * (1.0f / (1.0f + ((getStatAh(_stat_points->ah))/100.0f))); //=1 - (1 - (1/(1+(L10/100))))
}

void Player::_apply_acc(Team* allies){
	if ((_dyn_stats->next_acc - _dyn_stats->acc_ah_ticks + _dyn_stats->slow_ah_ticks) <= 0) {
		_dyn_stats->next_acc = _haste(getStatCdAcc(_stat_points->cd_acc));
		Player* target = _select_acc_target(allies);
		if (target == nullptr)
			return;
		else {
			target->getDynStats()->end_acc = (getStatAp(_stat_points->ap) + getStatAx(_stat_points->ax)) * getStatAccTicks(_stat_points->acc_ticks); // (ap + ax) * acc ticks
			target->getDynStats()->acc_as_ticks = getStatAs(target->_stat_points->as) * getStatAcc(_stat_points->acc); // ally_as * acc
			target->getDynStats()->acc_ah_ticks = getStatAh(target->_stat_points->ah) * getStatAcc(_stat_points->acc); // ally_ah * acc
		}
	} else
		_dyn_stats->next_acc--;
}

void Player::_apply_slow(Team* enemies){
	if ((_dyn_stats->next_slow - _dyn_stats->acc_ah_ticks + _dyn_stats->slow_ah_ticks) <= 0) {
		_dyn_stats->next_slow = _haste(getStatCdSlow(_stat_points->cd_slow));
		Player* target = _select_slow_target(enemies);
		if (target == nullptr)
			return;
		else {
			target->getDynStats()->end_slow = (int)roundf(((getStatAp(_stat_points->ap) + getStatAx(_stat_points->ax)) * getStatSlowTicks(_stat_points->slow_ticks)) * (1-(getStatTenacity(target->getStatPoints()->tenacity)))); // (ap + ax) * slow ticks * (1- tenacity)
			target->getDynStats()->slow_as_ticks = getStatAs(target->_stat_points->as) * getStatSlow(_stat_points->slow); // enemy_as * slow
			target->getDynStats()->slow_ah_ticks = getStatAh(target->_stat_points->ah) * getStatSlow(_stat_points->slow); // enemy_ah * slow
		}
	} else
		_dyn_stats->next_slow--;
}

void Player::_smite(Team* enemies){
	if ((_dyn_stats->next_smite - _dyn_stats->acc_ah_ticks + _dyn_stats->slow_ah_ticks) <= 0) {
		_dyn_stats->next_smite = _haste(getStatCdSmite(_stat_points->cd_smite));
		Player* target = _select_smite_target(enemies);
		if (target == nullptr)
			return;
		_damage_ap(target, (getStatAp(_stat_points->ap) + getStatAx(_stat_points->ax)) * (getStatSmite(_stat_points->smite)));
		}
	else _dyn_stats->next_smite--;
}

void Player::_blast(Team* enemies){
	if ((_dyn_stats->next_blast - _dyn_stats->acc_ah_ticks + _dyn_stats->slow_ah_ticks) <= 0) {
		_dyn_stats->next_blast = _haste(getStatCdBlast(_stat_points->cd_blast));
		int damage_output = (getStatAp(_stat_points->ap) + getStatAx(_stat_points->ax)) * (getStatBlast(_stat_points->blast));
		for (int i = 0; i < 5; i++) {
			Player* p = enemies->getPlayer(i);
			if (p && p->isAlive()) {
				_damage_ap(p, damage_output);
			}
			
		}
		
	}
	else _dyn_stats->next_blast--;
}

void Player::_heal(Team* allies){
	if ((_dyn_stats->next_heal - _dyn_stats->acc_ah_ticks + _dyn_stats->slow_ah_ticks) <= 0) {
		_dyn_stats->next_heal = _haste(getStatCdHeal(_stat_points->cd_heal));
		Player* target = _select_heal_target(allies);
		if (target == nullptr)
			return;
		_raw_heal(target, (getStatAp(_stat_points->ap) + getStatAx(_stat_points->ax)) * (getStatHeal(_stat_points->heal)));
		}
	else _dyn_stats->next_heal--;
}

void Player::_stun(Team* enemies){
	if ((_dyn_stats->next_stun - _dyn_stats->acc_ah_ticks + _dyn_stats->slow_ah_ticks) <= 0) {
		_dyn_stats->next_stun = _haste(getStatCdStun(_stat_points->cd_stun));
		Player* target = _select_stun_target(enemies);
		if (target == nullptr)
			return;
		else {
			target->getDynStats()->end_stun = (int)roundf((getStatAp(_stat_points->ap) + getStatAx(_stat_points->ax)) * getStatStun(_stat_points->stun) * (1 - (getStatTenacity(target->getStatPoints()->tenacity)))); // (ap + ax) * stun * (1-tenacity)
		} 
	} else
		_dyn_stats->next_stun--;
}

void Player::_shield(Team* allies){
	if ((_dyn_stats->next_shield - _dyn_stats->acc_ah_ticks + _dyn_stats->slow_ah_ticks) <= 0) {
		_dyn_stats->next_shield = _haste(getStatCdShield(_stat_points->cd_shield));
		Player* target = _select_shield_target(allies);
		if (target == nullptr)
			return;
		else {
			int total_ap = getStatAp(_stat_points->ap) + getStatAx(_stat_points->ax);
			target->getDynStats()->end_shield = total_ap * getStatShieldTicks(_stat_points->shield_ticks); // (ap + ax) * shield ticks
			target->getDynStats()->shield_resistance = total_ap * getStatShield(_stat_points->shield); // (ap + ax) * shield
		}
	} else
		_dyn_stats->next_shield--;
}

void Player::_mark(Team* enemies){
	if ((_dyn_stats->next_mark - _dyn_stats->acc_ah_ticks + _dyn_stats->slow_ah_ticks) <= 0) {
		_dyn_stats->next_mark = _haste(getStatCdMark(_stat_points->cd_mark));
		Player* target = _select_mark_target(enemies);
		if (target == nullptr)
			return;
		else {
			int total_ap = getStatAp(_stat_points->ap) + getStatAx(_stat_points->ax);
			target->getDynStats()->end_mark = (total_ap * getStatMarkTicks(_stat_points->mark_ticks)) * (1 - getStatTenacity(target->getStatPoints()->tenacity)); // ((ap + ax) * mark ticks) * (1-tenacity)
			target->getDynStats()->mark_resistance = total_ap * getStatMark(_stat_points->mark); // (ap + ax) * mark
		}
	} else
		_dyn_stats->next_mark--;
}

Player* Player::_select_attack_target(Team* enemies) {
    std::vector<Player*> alive_enemies;
    alive_enemies.reserve(5);
    for (int i = 0; i < 5; i++) {
        Player* p = enemies->getPlayer(i);
        if (p->isAlive()) {
            alive_enemies.push_back(p);
        }
    }
    if (alive_enemies.empty())
        return nullptr; // o lo que corresponda
    size_t n = alive_enemies.size();
    std::vector<int> reduced_hp(n);
    int denom = 0;
    for (size_t i = 0; i < n; i++) { // calcular hp reducida
        int hp = alive_enemies[i]->getDynStats()->hp;
        int red = _reduce_ad(alive_enemies[i], hp);
        reduced_hp[i] = red;
        denom += red;
    }
    if (denom == 0)
        denom = 1; // evitar división por cero
    std::vector<float> weights(n);
    for (size_t i = 0; i < n; i++) {
        float ratio = reduced_hp[i] / float(denom);
        weights[i] = 
            getStatAggro(alive_enemies[i]->getStatPoints()->aggro) +
            getStatFocus(_stat_points->focus) * (1.0f - ratio);
    }
    return alive_enemies[ linear_softmax(weights.data(), n) ];
}

Player* Player::_select_slow_target(Team* enemies) {
	std::vector<Player*> alive_enemies;
    alive_enemies.reserve(5);
    for (int i = 0; i < 5; i++) {
        Player* p = enemies->getPlayer(i);
        if (p->isAlive()) {
            alive_enemies.push_back(p);
        }
    }
    if (alive_enemies.empty())
        return nullptr; 
    size_t n = alive_enemies.size();
    std::vector<int> utility(n);
    int denom = 0;
    for (size_t i = 0; i < n; i++) { // calculate utility as ad+ap+ax+focus (heuristic, could change)
        int ad = getStatAd(alive_enemies[i]->getStatPoints()->ad);
        int ap = getStatAp(alive_enemies[i]->getStatPoints()->ap);
        int ax = getStatAx(alive_enemies[i]->getStatPoints()->ax);
        int foc = getStatFocus(alive_enemies[i]->getStatPoints()->focus);
        int enemy_utility = ad + ap + ax + foc;
        
        utility[i] = enemy_utility;
        denom += enemy_utility;
    }
    if (denom == 0)
        denom = 1; // evitar división entre cero
    std::vector<float> weights(n);
    for (size_t i = 0; i < n; i++) {
        float ratio = utility[i] / float(denom);
        weights[i] = getCounterFocus() + getStatFocus(_stat_points->focus) * (1.0f - ratio);
    }
    return alive_enemies[ linear_softmax(weights.data(), n) ];
}

Player* Player::_select_acc_target(Team* allies) {
	std::vector<Player*> alive_allies;
    alive_allies.reserve(5);
    for (int i = 0; i < 5; i++) {
        Player* p = allies->getPlayer(i);
        if (p->isAlive()) {
            alive_allies.push_back(p);
        }
    }
    if (alive_allies.empty())
        return nullptr; 
    size_t n = alive_allies.size();
    std::vector<int> utility(n);
    int denom = 0;
    for (size_t i = 0; i < n; i++) { // calculate utility as ad+ap+ax+focus (heuristic, could change)
        int ad = getStatAd(alive_allies[i]->getStatPoints()->ad);
        int ap = getStatAd(alive_allies[i]->getStatPoints()->ap);
        int ax = getStatAd(alive_allies[i]->getStatPoints()->ax);
        int foc = getStatFocus(alive_allies[i]->getStatPoints()->focus);
        int ally_utility = ad + ap + ax + foc;
        
        utility[i] = ally_utility;
        denom += ally_utility;
    }
    if (denom == 0)
        denom = 1; // evitar división entre cero
    std::vector<float> weights(n);
    for (size_t i = 0; i < n; i++) {
        float ratio = utility[i] / float(denom);
        weights[i] = getCounterFocus() + getStatFocus(_stat_points->focus) * (1.0f - ratio);
    }
    return alive_allies[ linear_softmax(weights.data(), n) ];
}

Player* Player::_select_smite_target(Team* enemies) {
    std::vector<Player*> alive_enemies;
    alive_enemies.reserve(5);
    for (int i = 0; i < 5; i++) {
        Player* p = enemies->getPlayer(i);
        if (p->isAlive()) {
            alive_enemies.push_back(p);
        }
    }
    if (alive_enemies.empty())
        return nullptr; // o lo que corresponda
    size_t n = alive_enemies.size();
    std::vector<int> reduced_hp(n);
    int denom = 0;
    for (size_t i = 0; i < n; i++) { // calcular hp reducida
        int hp = alive_enemies[i]->getDynStats()->hp;
        int red = _reduce_ap(alive_enemies[i], hp);
        reduced_hp[i] = red;
        denom += red;
    }
    if (denom == 0)
        denom = 1; // evitar división por cero
    std::vector<float> weights(n);
    for (size_t i = 0; i < n; i++) {
        float ratio = reduced_hp[i] / float(denom);
        weights[i] = 
            getStatAggro(alive_enemies[i]->getStatPoints()->aggro) +
            getStatFocus(_stat_points->focus) * (1.0f - ratio);
    }
    return alive_enemies[ linear_softmax(weights.data(), n) ];
}

Player* Player::_select_heal_target(Team* allies) {
	std::vector<Player*> alive_allies;
    alive_allies.reserve(5);
    for (int i = 0; i < 5; i++) {
        Player* p = allies->getPlayer(i);
        if (p->isAlive()) {
            alive_allies.push_back(p);
        }
    }
    if (alive_allies.empty())
        return nullptr; 
    size_t n = alive_allies.size();
    std::vector<float> frailty(n);
	float denom = 0.0f;
	for (size_t i = 0; i < n; ++i) {
		float hp    = static_cast<float>(alive_allies[i]->getDynStats()->hp);
		float maxhp = static_cast<float>(getStatMaxHp(alive_allies[i]->getStatPoints()->max_hp));
		float ally_frailty = (maxhp > 0.0f) ? (hp / maxhp) : 1.0f; // 1 = full, 0 = muerto
		frailty[i] = ally_frailty;
		denom += ally_frailty;
	}
	if (denom <= 0.0f)
		denom = 1.0f;

    std::vector<float> weights(n);
    for (size_t i = 0; i < n; i++) {
        float ratio = frailty[i] / float(denom);
        weights[i] = getCounterFocus() + getStatFocus(_stat_points->focus) * (1.0f - ratio);
    }
    return alive_allies[ linear_softmax(weights.data(), n) ];
}


Player* Player::_select_stun_target(Team* enemies) {
	std::vector<Player*> alive_enemies;
    alive_enemies.reserve(5);
    for (int i = 0; i < 5; i++) {
        Player* p = enemies->getPlayer(i);
        if (p->isAlive()) {
            alive_enemies.push_back(p);
        }
    }
    if (alive_enemies.empty())
        return nullptr; 
    size_t n = alive_enemies.size();
    std::vector<int> utility(n);
    int denom = 0;
    for (size_t i = 0; i < n; i++) { // calculate utility as ad+ap+ax+focus (heuristic, could change)
        int ad = getStatAd(alive_enemies[i]->getStatPoints()->ad);
        int ap = getStatAd(alive_enemies[i]->getStatPoints()->ap);
        int ax = getStatAd(alive_enemies[i]->getStatPoints()->ax);
        int foc = getStatFocus(alive_enemies[i]->getStatPoints()->focus);
        int enemy_utility = ad + ap + ax + foc;
        
        utility[i] = enemy_utility;
        denom += enemy_utility;
    }
    if (denom == 0)
        denom = 1; // evitar división entre cero
    std::vector<float> weights(n);
    for (size_t i = 0; i < n; i++) {
        float ratio = utility[i] / float(denom);
        weights[i] = getCounterFocus() + getStatFocus(_stat_points->focus) * (1.0f - ratio);
    }
    return alive_enemies[ linear_softmax(weights.data(), n) ];
}


Player* Player::_select_shield_target(Team* allies) {
	std::vector<Player*> alive_allies;
    alive_allies.reserve(5);
    for (int i = 0; i < 5; i++) {
        Player* p = allies->getPlayer(i);
        if (p->isAlive()) {
            alive_allies.push_back(p);
        }
    }
    if (alive_allies.empty())
        return nullptr; 
    size_t n = alive_allies.size();
    std::vector<float> frailty(n);
	float denom = 0.0f;
	for (size_t i = 0; i < n; ++i) {
		float hp    = static_cast<float>(alive_allies[i]->getDynStats()->hp);
		float maxhp = static_cast<float>(getStatMaxHp(alive_allies[i]->getStatPoints()->max_hp));
		float ally_frailty = (maxhp > 0.0f) ? (hp / maxhp) : 1.0f; // 1 = full, 0 = muerto
		frailty[i] = ally_frailty;
		denom += ally_frailty;
	}
	if (denom <= 0.0f)
		denom = 1.0f;

    std::vector<float> weights(n);
    for (size_t i = 0; i < n; i++) {
        float ratio = frailty[i] / float(denom);
        weights[i] = getCounterFocus() + getStatFocus(_stat_points->focus) * (1.0f - ratio);
    }
    return alive_allies[ linear_softmax(weights.data(), n) ];
}


Player* Player::_select_mark_target(Team* enemies) {
	std::vector<Player*> alive_enemies;
    alive_enemies.reserve(5);
    for (int i = 0; i < 5; i++) {
        Player* p = enemies->getPlayer(i);
        if (p->isAlive()) {
            alive_enemies.push_back(p);
        }
    }
    if (alive_enemies.empty())
        return nullptr; 
    size_t n = alive_enemies.size();
    std::vector<float> frailty(n);
	float denom = 0.0f;
	for (size_t i = 0; i < n; ++i) {
		float hp    = static_cast<float>(alive_enemies[i]->getDynStats()->hp);
		float maxhp = static_cast<float>(getStatMaxHp(alive_enemies[i]->getStatPoints()->max_hp));
		float enemy_frailty = (maxhp > 0.0f) ? (hp / maxhp) : 1.0f; // 1 = full, 0 = muerto
		frailty[i] = enemy_frailty;
		denom += enemy_frailty;
	}
	if (denom <= 0.0f)
		denom = 1.0f;

    std::vector<float> weights(n);
    for (size_t i = 0; i < n; i++) {
        float ratio = frailty[i] / float(denom);
        weights[i] = getCounterFocus() + getStatFocus(_stat_points->focus) * (1.0f - ratio);
    }
    return alive_enemies[ linear_softmax(weights.data(), n) ];
}

void Player::_randomize_stats() {
    int* stats[] = {
        &_stat_points->max_hp,
        &_stat_points->regen,
        &_stat_points->ad,
        &_stat_points->armor,
        &_stat_points->armor_pen,
        &_stat_points->lethality,
        &_stat_points->as,
        &_stat_points->crit,
        &_stat_points->crit_factor,
        &_stat_points->bleed,
        &_stat_points->bleed_dmg,
        &_stat_points->bleed_ticks,
        &_stat_points->ap,
        &_stat_points->mr,
        &_stat_points->mr_pen,
        &_stat_points->ethereal,
        &_stat_points->ah,
        &_stat_points->smite,
        &_stat_points->cd_smite,
        &_stat_points->blast,
        &_stat_points->cd_blast,
        &_stat_points->heal,
        &_stat_points->cd_heal,
        &_stat_points->stun,
        &_stat_points->cd_stun,
        &_stat_points->acc,
        &_stat_points->acc_ticks,
        &_stat_points->cd_acc,
        &_stat_points->slow,
        &_stat_points->slow_ticks,
        &_stat_points->cd_slow,
        &_stat_points->shield,
        &_stat_points->shield_ticks,
        &_stat_points->cd_shield,
        &_stat_points->mark,
        &_stat_points->mark_ticks,
        &_stat_points->cd_mark,
        &_stat_points->vamp,
        &_stat_points->thorns,
        &_stat_points->ax,
        &_stat_points->tenacity,
        &_stat_points->aggro,
        &_stat_points->focus
    };
    constexpr int N = sizeof(stats) / sizeof(stats[0]);
		
    for (int i = 0; i < N; i++) {
			*stats[i] = 0;
		}

    for (int p = 0; p < 100; p++) {
        int index = rng::randint(0, N - 1); 
        *stats[index] += 1;                
    }
}


void Player::print() {
    std::cout << "Player ID: " << _player_id << "\n";

    std::cout << "=== StatPoints ===\n";
    std::cout << "max_hp: " <<_stat_points->max_hp << "\n";
    std::cout << "regen: " <<_stat_points->regen << "\n";
    std::cout << "ad: " <<_stat_points->ad << "\n";
    std::cout << "armor: " <<_stat_points->armor << "\n";
    std::cout << "armor_pen: " <<_stat_points->armor_pen << "\n";
    std::cout << "lethality: " <<_stat_points->lethality << "\n";
    std::cout << "as: " <<_stat_points->as << "\n";
    std::cout << "crit: " <<_stat_points->crit << "\n";
    std::cout << "crit_factor: " <<_stat_points->crit_factor << "\n";
    std::cout << "bleed: " <<_stat_points->bleed << "\n";
    std::cout << "bleed_dmg: " <<_stat_points->bleed_dmg << "\n";
    std::cout << "bleed_ticks: " <<_stat_points->bleed_ticks << "\n";
    std::cout << "ap: " <<_stat_points->ap << "\n";
    std::cout << "mr: " <<_stat_points->mr << "\n";
    std::cout << "mr_pen: " <<_stat_points->mr_pen << "\n";
    std::cout << "ethereal: " <<_stat_points->ethereal << "\n";
    std::cout << "ah: " <<_stat_points->ah << "\n";
    std::cout << "smite: " <<_stat_points->smite << "\n";
    std::cout << "cd_smite: " <<_stat_points->cd_smite << "\n";
    std::cout << "blast: " <<_stat_points->blast << "\n";
    std::cout << "cd_blast: " <<_stat_points->cd_blast << "\n";
    std::cout << "heal: " <<_stat_points->heal << "\n";
    std::cout << "cd_heal: " <<_stat_points->cd_heal << "\n";
    std::cout << "stun: " <<_stat_points->stun << "\n";
    std::cout << "cd_stun: " <<_stat_points->cd_stun << "\n";
    std::cout << "acc: " <<_stat_points->acc << "\n";
    std::cout << "acc_ticks: " <<_stat_points->acc_ticks << "\n";
    std::cout << "cd_acc: " <<_stat_points->cd_acc << "\n";
    std::cout << "slow: " <<_stat_points->slow << "\n";
    std::cout << "slow_ticks: " <<_stat_points->slow_ticks << "\n";
    std::cout << "cd_slow: " <<_stat_points->cd_slow << "\n";
    std::cout << "shield: " <<_stat_points->shield << "\n";
    std::cout << "shield_ticks: " <<_stat_points->shield_ticks << "\n";
    std::cout << "cd_shield: " <<_stat_points->cd_shield << "\n";
    std::cout << "mark: " <<_stat_points->mark << "\n";
    std::cout << "mark_ticks: " <<_stat_points->mark_ticks << "\n";
    std::cout << "cd_mark: " <<_stat_points->cd_mark << "\n";
    std::cout << "vamp: " <<_stat_points->vamp << "\n";
    std::cout << "thorns: " <<_stat_points->thorns << "\n";
    std::cout << "ax: " <<_stat_points->ax << "\n";
    std::cout << "tenacity: " <<_stat_points->tenacity << "\n";
    std::cout << "aggro: " <<_stat_points->aggro << "\n";
    std::cout << "focus: " <<_stat_points->focus << "\n";

    std::cout << "=== DynamicStats ===\n";
    std::cout << "hp: " <<_dyn_stats->hp << "\n";
    std::cout << "next_regen: " <<_dyn_stats->next_regen << "\n";
    std::cout << "next_attack: " <<_dyn_stats->next_attack << "\n";
    std::cout << "acc_as_ticks: " <<_dyn_stats->acc_as_ticks << "\n";
    std::cout << "slow_as_ticks: " <<_dyn_stats->slow_as_ticks << "\n";
    std::cout << "end_acc: " <<_dyn_stats->end_acc << "\n";
    std::cout << "end_slow: " <<_dyn_stats->end_slow << "\n";
    std::cout << "end_shield: " <<_dyn_stats->end_shield << "\n";
    std::cout << "end_mark: " <<_dyn_stats->end_mark << "\n";
    std::cout << "shield_resistance: " <<_dyn_stats->shield_resistance << "\n";
    std::cout << "mark_resistance: " <<_dyn_stats->mark_resistance << "\n";
}


void Player::_init_player() {
    this->_is_alive = true;
    this->_dyn_stats->hp = getStatMaxHp(_stat_points->max_hp);
    this->_dyn_stats->next_regen = 50;
    this->_dyn_stats->next_attack = getStatAs(_stat_points->as);
    this->_dyn_stats->next_bleed = 0;
    this->_dyn_stats->next_acc = _haste(getStatCdAcc(_stat_points->cd_acc));
    this->_dyn_stats->next_slow = _haste(getStatCdSlow(_stat_points->cd_slow));
    this->_dyn_stats->next_smite = _haste(getStatCdSmite(_stat_points->cd_smite));
    this->_dyn_stats->next_blast = _haste(getStatCdBlast(_stat_points->cd_blast));
    this->_dyn_stats->next_heal = _haste(getStatCdHeal(_stat_points->cd_heal));
    this->_dyn_stats->next_stun = _haste(getStatCdStun(_stat_points->cd_stun));
    this->_dyn_stats->next_shield = _haste(getStatCdShield(_stat_points->cd_shield));
    this->_dyn_stats->next_mark = _haste(getStatCdMark(_stat_points->cd_mark));
    
    
    this->_dyn_stats->acc_as_ticks  = 0;
    this->_dyn_stats->acc_ah_ticks  = 0;
    this->_dyn_stats->slow_as_ticks = 0;
    this->_dyn_stats->slow_ah_ticks = 0;
    
    this->_dyn_stats->end_acc = 0;
    this->_dyn_stats->end_slow = 0;
    this->_dyn_stats->end_shield = 0;
    this->_dyn_stats->end_mark = 0;
    this->_dyn_stats->end_stun = 0;
    this->_dyn_stats->shield_resistance = 0;
    this->_dyn_stats->mark_resistance = 0;
    this->_dyn_stats->bleed_stacks = 0;
    this->_dyn_stats->bleed_accumulated_damage = 0;
    this->_dyn_stats->end_bleed = 0;
    
    this->_dyn_stats->track_damage_dealt = 0;
    this->_dyn_stats->track_damage_received = 0;
    this->_dyn_stats->track_hp_raw_healed = 0;
    this->_dyn_stats->track_healed_this_tick = false;
    //TODO agregar el resto
}
