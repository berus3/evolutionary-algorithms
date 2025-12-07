#ifndef PLAYER_HPP
#define PLAYER_HPP

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

struct Stats { // TODO: seleccionar las stats que no son estaticas durante una misma pelea
    float hp;
    float regen;
    float ad;
    float armor;
    float armor_pen;
    float lethality;
    float as;
    float crit;
    float crit_factor;
    float bleed;
    float bleed_dmg;
    float bleed_ticks;
    float ap;
    float mr;
    float mr_pen;
    float ethereal;
    float ah;
    float smite;
    float cd_smite;
    float blast;
    float cd_blast;
    float heal;
    float cd_heal;
    float stun;
    float cd_stun;
    float acc;
    float acc_ticks;
    float cd_acc;
    float slow;
    float slow_ticks;
    float cd_slow;
    float shield;
    float shield_ticks;
    float cd_shield;
    float mark;
    float mark_ticks;
    float cd_mark;
    float vamp;
    float thorns;
    float ax;
    float tenacity;
    float aggro;
    float focus;
};

class Team; // forward declaration

class Player {
  private:
      int player_id;
      StatPoints* stat_points;
      Stats* stats;
      bool is_alive;
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