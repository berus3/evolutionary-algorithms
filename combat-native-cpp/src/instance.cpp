#include "../include/instance.hpp"
#include <math.h>

/*
float getStatStat(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED:
            return 100.0f + stat_point * 20.0f;
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}
*/

float getStatMaxHp(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: { //f(x)=1600+9385 (1-ℯ^(-0.02257 x))
            const float k = 0.02257f;
            const float B = 9385.0f;
            return 1600.0f + B * (1.0f - expf(-k * stat_point));
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

float getStatRegen(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: { //f(x)=5+428.571 (1-ℯ^(-0.016425 x))
            const float k = 0.016425f;
            const float B = 428.571f;
            return 5.0f + B * (1.0f - expf(-k * stat_point));
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

float getStatAd(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 50 + 2861.85 * (1 - e^(-0.004565 x))
            const float k = 0.004565f;
            const float B = 2861.85f;
            return 50.0f + B * (1.0f - expf(-k * stat_point));
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

float getStatArmor(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.00875 * x^2 + 7.325 * x + 80
            return 0.00875f * stat_point * stat_point
                 + 7.325f * stat_point
                 + 80.0f;
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

float getStatArmorPen(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.4005 * (1 - exp(-0.06915 * x))
            const float L = 0.4005f;
            const float k = 0.06915f;
            return L * (1.0f - expf(-k * stat_point));
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

float getStatLethality(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 110.76 * (1 - exp(-0.05 * x))
            const float L = 110.76f;
            const float k = 0.05f;
            return L * (1.0f - expf(-k * stat_point));
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

float getStatAs(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 24.42 + 135.58 * exp(-0.13786 * x)
            const float A = 24.42f;
            const float B = 135.58f;
            const float k = 0.13786f;
            return A + B * expf(-k * stat_point);
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

float getStatCrit(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 100 * (1 - exp(-0.0693147 * x))
            const float L = 100.0f;
            const float k = 0.0693147f;
            return L * (1.0f - expf(-k * stat_point));
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

float getStatCritFactor(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 2.85 - 1.35 * exp(-0.0463733 * x)
            const float L = 2.85f;
            const float B = 1.35f;
            const float k = 0.0463733f;
            return L - B * expf(-k * stat_point);
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

//bleed

//bleed dmg

//bleed ticks

float getStatAp(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 1597.9 * (1 - exp(-0.02354 * x))
            const float L = 1597.9f;
            const float k = 0.02354f;
            return L * (1.0f - expf(-k * stat_point));
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}



// TODO: Implement other stat functions similarly
