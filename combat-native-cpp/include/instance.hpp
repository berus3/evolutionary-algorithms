#ifndef INSTANCE_HPP
#define INSTANCE_HPP

enum Instance {
    BALANCED,
    LINEAR,
    UNFAIR
};

float getStatMaxHp(int stat_point, Instance instance);
float getStatRegen(int stat_point, Instance instance);
float getStatAd(int stat_point, Instance instance);
float getStatArmor(int stat_point, Instance instance);
float getStatArmorPen(int stat_point, Instance instance);
float getStatLethality(int stat_point, Instance instance);
float getStatAs(int stat_point, Instance instance);
float getStatCrit(int stat_point, Instance instance);
float getStatCritFactor(int stat_point, Instance instance);
float getStatBleed(int stat_point, Instance instance);
float getStatBleedDmg(int stat_point, Instance instance);
float getStatBleedTicks(int stat_point, Instance instance);
float getStatAp(int stat_point, Instance instance);
float getStatMr(int stat_point, Instance instance);
float getStatMrPen(int stat_point, Instance instance);
float getStatEthereal(int stat_point, Instance instance);
float getStatAh(int stat_point, Instance instance);
float getStatSmite(int stat_point, Instance instance);
float getStatCdSmite(int stat_point, Instance instance);
float getStatBlast(int stat_point, Instance instance);
float getStatCdBlast(int stat_point, Instance instance);
float getStatHeal(int stat_point, Instance instance);
float getStatCdHeal(int stat_point, Instance instance);
float getStatStun(int stat_point, Instance instance);
float getStatCdStun(int stat_point, Instance instance);
float getStatAcc(int stat_point, Instance instance);
float getStatAccTicks(int stat_point, Instance instance);
float getStatCdAcc(int stat_point, Instance instance);
float getStatSlow(int stat_point, Instance instance);
float getStatSlowTicks(int stat_point, Instance instance);
float getStatCdSlow(int stat_point, Instance instance);
float getStatShield(int stat_point, Instance instance);
float getStatShieldTicks(int stat_point, Instance instance);
float getStatCdShield(int stat_point, Instance instance);
float getStatMark(int stat_point, Instance instance);
float getStatMarkTicks(int stat_point, Instance instance);
float getStatCdMark(int stat_point, Instance instance);
float getStatVamp(int stat_point, Instance instance);
float getStatThorns(int stat_point, Instance instance);
float getStatAx(int stat_point, Instance instance);
float getStatTenacity(int stat_point, Instance instance);
float getStatAggro(int stat_point, Instance instance);
float getStatFocus(int stat_point, Instance instance);

#endif // INSTANCE_HPP