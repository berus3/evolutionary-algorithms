#include "player.hpp"

Player::Player(int id) { this->player_id = id; }
Player::~Player() {}
int Player::getId() { return player_id; }
StatPoints* Player::getStatPoints() { return stat_points; }
void Player::setStatPoints(StatPoints* sp) { stat_points = sp; }
void Player::act(Team* allies, Team* enemies) {

}