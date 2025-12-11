#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "instance.hpp"
#include <vector>

struct StatPoints {
    int max_hp;
    int regen;
    int ad;
    int armor;
    int armor_pen;
    int lethality;
    int as;
    int crit;
    int crit_factor;
    int bleed;
    int bleed_dmg;
    int bleed_ticks;
    int ap;
    int mr;
    int mr_pen;
    int ethereal;
    int ah;
    int smite;
    int cd_smite;
    int blast;
    int cd_blast;
    int heal;
    int cd_heal;
    int stun;
    int cd_stun;
    int acc;
    int acc_as_ticks;
    int cd_acc;
    int slow;
    int slow_as_ticks;
    int cd_slow;
    int shield;
    int shield_ticks;
    int cd_shield;
    int mark;
    int mark_ticks;
    int cd_mark;
    int vamp;
    int thorns;
    int ax;
    int tenacity;
    int aggro;
    int focus;
};

struct DynamicStats { // TODO: seleccionar las stats que no son estaticas durante una misma pelea
    int hp;
    int next_regen;
    int next_attack;
    int next_bleed; //received, not dealt
    int next_acc;
    
    int acc_as_ticks;
    int acc_ah_ticks;
    int slow_as_ticks;
    int slow_ah_ticks;
    int end_acc;
    int end_slow;
    
    int end_shield;
    int end_mark;
    int shield_resistance;
    int mark_resistance;
    
    int bleed_stacks;
    int bleed_accumulated_damage;
    int end_bleed;
    //TODO implemetar funciones para darle shield resistance y mark resistance a personajes
    //TODO implementar funciones de slow y acc (darle slow y acc a otro personaje)
};

class Team; // forward declaration

class Player {
  private:
      int _player_id;
      StatPoints* _stat_points;
      DynamicStats* _dyn_stats;
      bool _is_alive;
      void _update_effects();
      void _regen();
      void _attack(Team* enemies);
      Player* _select_attack_target(Team* enemies);
      Player* _select_acc_target(Team* allies);
      void _heal(Player*, int healing);
      int _apply_crit(int damage);
      void _damage_ad(Player* player, int damage_output);
      void _damage_ap(Player* player, int damage_output);
      void _bleed(Player* player, int damage_dealt);
      void _receive_bleed();
      void _damage_bleed(Player* player, int damage_output);
      int _reduce_ad(Player* target, int damage_output);
      int _reduce_ap(Player* target, int damage_output);
      void _apply_acc(Team* allies);
      int _haste(int);
      
      void _init_player();
      void _randomize_stats();
  public:
      Player(int id);
      Player();
      ~Player();
      int getId();
      StatPoints* getStatPoints();
      bool isAlive() {return _is_alive;}
      void setStatPoints(StatPoints* sp);
      DynamicStats* getDynStats();
      void setDynStats(DynamicStats* ds);
      void print();
      void act(Team* allies, Team* enemies);
      int lastAttacked;
      // TODO: Add methods to get/set Stats
};

#endif // PLAYER_HPP
