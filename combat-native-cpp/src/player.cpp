#include "player.hpp"

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
        is_alive = stats->hp > 0;
        stats->hp += stats->regen;
        // regen
        // attack
        // smite
        // blast
        // heal
        // stun
        // acc
        // slow
        // shield
        // mark
    } else {
        stats->hp = 0;
    }
}