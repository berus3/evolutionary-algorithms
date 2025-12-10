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

int getStatMaxHp(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: { //f(x)=1600+9385 (1-ℯ^(-0.02257 x))
            const float k = 0.02257f;
            const float B = 9385.0f;
            return (int)roundf(1600.0f + B * (1.0f - expf(-k * stat_point)));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatRegen(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: { //f(x)=5+428.571 (1-ℯ^(-0.016425 x))
            const float k = 0.016425f;
            const float B = 428.571f;
            return (int)roundf(5.0f + B * (1.0f - expf(-k * stat_point)));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatAd(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 50 + 2861.85 * (1 - e^(-0.004565 x))
            const float k = 0.004565f;
            const float B = 2861.85f;
            return (int)roundf(50.0f + B * (1.0f - expf(-k * stat_point)));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatArmor(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.00875 * x^2 + 7.325 * x + 80
            return (int)roundf(0.00875f * stat_point * stat_point
                 + 7.325f * stat_point
                 + 80.0f);
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatArmorPen(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.4005 * (1 - exp(-0.06915 * x))
            const float L = 0.4005f;
            const float k = 0.06915f;
            return (int)roundf(L * (1.0f - expf(-k * stat_point)));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatLethality(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 110.76 * (1 - exp(-0.05 * x))
            const float L = 110.76f;
            const float k = 0.05f;
            return (int)roundf(L * (1.0f - expf(-k * stat_point)));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatAs(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 150 + 450 * exp(-0.038664 * x)
            const float C = 30.0f;
            const float D = 100.0f;
            const float k = 0.048664f;
            return (int)roundf(C + D * expf(-k * stat_point));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatCrit(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 100 * (1 - exp(-0.0693147 * x))
            const float L = 100.0f;
            const float k = 0.0693147f;
            return (int)roundf(L * (1.0f - expf(-k * stat_point)));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
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

//check P(bleed) > 1 --> bleed
float getStatBleed(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.02252 * x^0.8239
            const float B = 0.02252f;
            const float p = 0.8239f;
            return B * powf(stat_point, p);
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}


int getStatBleedDmg(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.05 + 0.0182 * x^0.569
            const float A = 0.05f;
            const float B = 0.0182f;
            const float p = 0.569f;
            return (int)roundf(A + B * powf(stat_point, p));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatBleedTicks(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 120 + 8.35 * x^0.829
            const float A = 120.0f;
            const float B = 8.35f;
            const float p = 0.829f;
            return (int)roundf(A + B * powf(stat_point, p));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}



int getStatAp(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 1597.9 * (1 - exp(-0.02354 * x))
            const float L = 1597.9f;
            const float k = 0.02354f;
            return (int)roundf(L * (1.0f - expf(-k * stat_point)));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatMr(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 43 + 12.59 * x^0.7266
            const float A = 43.0f;
            const float B = 12.59f;
            const float p = 0.7266f;
            return (int)roundf(A + B * powf(stat_point, p));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}


int getStatMrPen(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.4005 * (1 - exp(-0.06915 * x))
            const float L = 0.4005f;
            const float k = 0.06915f;
            return (int)roundf(L * (1.0f - expf(-k * stat_point)));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatEthereal(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 10.79 * x^0.22185
            const float B = 10.79f;
            const float p = 0.22185f;
            return (int)roundf(B * powf(stat_point, p));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatAh(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 14.25 * x^0.4804
            const float B = 14.25f;
            const float p = 0.4804f;
            return (int)roundf(B * powf(stat_point, p));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatSmite(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = A * (e^{-b e^{-c x}} - e^{-b}) / (1 - e^{-b})
            const float A = 3.0f;
            const float b = 5.7f;
            const float c = 0.073f;

            float num   = expf(-b * expf(-c * stat_point)) - expf(-b);
            float denom = 1.0f - expf(-b);
            return A * (num / denom);
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

int getStatCdSmite(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x)=150+550 e^(-0.0313 x)
            const float C = 150.0f;
            const float D = 550.0f;
            const float k = 0.0313f;
            return (int)roundf(C + D * expf(-k * stat_point));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatBlast(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = A * (e^{-b e^{-c x}} - e^{-b}) / (1 - e^{-b})
            const float A = 1.0f;
            const float b = 5.7f;
            const float c = 0.073f;

            float num   = expf(-b * expf(-c * stat_point)) - expf(-b);
            float denom = 1.0f - expf(-b);
            return A * (num / denom);
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}


int getStatCdBlast(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 500 + 700 * exp(-0.0693 * x)
            const float C = 400.0f;
            const float D = 800.0f;
            const float k = 0.0393f;
            return (int)roundf(C + D * expf(-k * stat_point));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}


float getStatHeal(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = A * (e^{-b e^{-c x}} - e^{-b}) / (1 - e^{-b})
            const float A = 3.0f;
            const float b = 5.7f;
            const float c = 0.073f;

            float num   = expf(-b * expf(-c * stat_point)) - expf(-b);
            float denom = 1.0f - expf(-b);
            return A * (num / denom);
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}


int getStatCdHeal(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 150 + 450 * exp(-0.038664 * x)
            const float C = 150.0f;
            const float D = 450.0f;
            const float k = 0.038664f;
            return (int)roundf(C + D * expf(-k * stat_point));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatStun(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = A * (e^{-b e^{-c x}} - e^{-b}) / (1 - e^{-b})
            const float A = 2.0f;
            const float b = 5.7f;
            const float c = 0.073f;

            float num   = expf(-b * expf(-c * stat_point)) - expf(-b);
            float denom = 1.0f - expf(-b);
            return A * (num / denom);
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}


int getStatCdStun(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 300 + 700 * exp(-0.038664 * x)
            const float C = 300.0f;
            const float D = 700.0f;
            const float k = 0.038664f;
            return (int)roundf(C + D * expf(-k * stat_point));
        }
        case LINEAR:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case UNFAIR:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatAcc(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = A * (e^{-b e^{-c x}} - e^{-b}) / (1 - e^{-b})
            const float A = 1.0f;
            const float b = 5.7f;
            const float c = 0.173f;

            float num   = expf(-b * expf(-c * stat_point)) - expf(-b);
            float denom = 1.0f - expf(-b);
            return A * (num / denom);
        }
        case LINEAR:
            return 80.0f + stat_point * 25.0f;
        case UNFAIR:
            return 120.0f + stat_point * 15.0f;
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

//depende del ap
float getStatAccTicks(int stat_point, Instance instance) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.8 + 0.117 * x^0.777
            const float A = 0.8f;
            const float B = 0.117f;
            const float p = 0.777f;
            return A + B * powf(stat_point, p);
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
