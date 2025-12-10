#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <math.h>

enum Instance {
    BALANCED,
    LINEAR,
    UNFAIR
};

extern Instance instance;

int getStatMaxHp(int stat_point);
int getStatRegen(int stat_point);
int getStatAd(int stat_point);
int getStatArmor(int stat_point);
int getStatArmorPen(int stat_point);
int getStatLethality(int stat_point);
int getStatAs(int stat_point);
float getStatCrit(int stat_point);
float getStatCritFactor(int stat_point);
float getStatBleed(int stat_point);
int getStatBleedDmg(int stat_point);
int getStatBleedTicks(int stat_point);
int getStatAp(int stat_point);
int getStatMr(int stat_point);
int getStatMrPen(int stat_point);
int getStatEthereal(int stat_point);
int getStatAh(int stat_point);
float getStatSmite(int stat_point);
int getStatCdSmite(int stat_point);
float getStatBlast(int stat_point);
int getStatCdBlast(int stat_point);
float getStatHeal(int stat_point);
int getStatCdHeal(int stat_point);
float getStatStun(int stat_point);
int getStatCdStun(int stat_point);
float getStatAcc(int stat_point);
float getStatAccTicks(int stat_point);
int getStatCdAcc(int stat_point);
float getStatSlow(int stat_point);
float getStatSlowTicks(int stat_point);
int getStatCdSlow(int stat_point);
float getStatShield(int stat_point);
int getStatShieldTicks(int stat_point);
int getStatCdShield(int stat_point);
float getStatMark(int stat_point);
float getStatMarkTicks(int stat_point);
int getStatCdMark(int stat_point);
float getStatVamp(int stat_point);
float getStatThorns(int stat_point);
int getStatAx(int stat_point);
float getStatTenacity(int stat_point);
int getStatAggro(int stat_point);
int getStatFocus(int stat_point);

#endif // INSTANCE_HPP
