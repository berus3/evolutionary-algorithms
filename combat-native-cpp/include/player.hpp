#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "instance.hpp"

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
    int acc_ticks;
    int cd_acc;
    int slow;
    int slow_ticks;
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
    int acc_ticks;
    int slow_ticks;
    int end_acc;
    int end_slow;
    //TODO implementar funciones de slow y acc (darle slow y acc a otro personaje)
};

class Team; // forward declaration

class Player {
  private:
      int player_id;
      StatPoints* stat_points;
      DynamicStats* dyn_stats;
      bool is_alive;
      void update_effects();
      void regen();
      void attack(Team* enemies);
      Player* selectAttackTarget(Team* enemies);
      void heal(int healing);
      int apply_crit(int damage);
      void damage_ad(Player* player);
      void damage_ap(Player* player);
      void bleed(Player* player);
  public:
      Player(int id);
      ~Player();
      int getId();
      StatPoints* getStatPoints();
      void setStatPoints(StatPoints* sp);
      void act(Team* allies, Team* enemies);
      // TODO: Add methods to get/set Stats
};

#endif // PLAYER_HPP
