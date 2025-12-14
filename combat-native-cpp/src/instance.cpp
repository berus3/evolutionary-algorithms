#include "../include/instance.hpp"


Instance instance;


int linear_softmax(const float* weights, int n, uint64_t ctx) {
    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        sum += weights[i];
    }
    double r = rng::real01(ctx);
    float acc = 0.0f;
    for (int i = 0; i < n; i++) {
        acc += weights[i] / sum;
        if (r <= acc) {
            return i;
        }
    }
    return n - 1;
}

float getCounterFocus() {
    switch (instance) {
        case BALANCED: return 10.0f;
        case PIECEWISE: return 10.0f;
        case EXPONENTIAL:return 10.0f;
        default: return 10.0f;
    }
}

float stat_piecewise_exp_linear(
    int x,               // stat_point
    int p,               // inflection_point
    float y0,            // f(x = 0)
    float yp,            // f(x = p)
    float y100,          // f(x = 100)
    float k = 0.08f      // exponential growth (0.05–0.15)
) {
    if (x <= 0) return y0;
    if (x >= 100) return y100;
    // k = 2.0f;
    // k = 2.0f;
    if (x <= p) {
        float num = std::exp(k * x) - 1.0f;
        float den = std::exp(k * p) - 1.0f;
        return y0 + (yp - y0) * (num / den);
    } else {
        return yp + (y100 - yp) * (x - p) / float(100 - p);
    }
}

float stat_exponential(
    int x,
    float y0,
    float y20,
    float k = 0.02f
) {
    const float xmax = 20.0f;

    float num   = expf(k * x) - 1.0f;
    float denom = expf(k * xmax) - 1.0f;

    return y0 + (y20 - y0) * (num / denom);
}


float stat_exponential(
    int x,
    float y0,
    float y20,
    float k = 0.02f
) {
    const float xmax = 20.0f;

    float num   = expf(k * x) - 1.0f;
    float denom = expf(k * xmax) - 1.0f;

    return y0 + (y20 - y0) * (num / denom);
}


int getStatMaxHp(int stat_point) {
    switch (instance) {
        case BALANCED: { //f(x)=1600+9385 (1-ℯ^(-0.02257 x))
            const float k = 0.02257f;
            const float B = 9385.0f;
            return (int)roundf(1600.0f + B * (1.0f - expf(-k * stat_point)));
        }
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 1600.0f;
            const float yp = 6000.0f;
            const float y100 = 10000.0f;
            const float k = 0.1f;
            return (int)std::floor(stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k) + 0.5);
        }
        case EXPONENTIAL: {
            const float y0 = 1600.0f;
            const float yp = 6000.0f;
            return (int)roundf(stat_exponential(stat_point, y0, yp));
        }
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatRegen(int stat_point) {
    switch (instance) {
        case BALANCED: { //f(x)=5+428.571 (1-ℯ^(-0.016425 x))
            const float k = 0.016425f;
            const float B = 428.571f;
            return (int)roundf(5.0f + B * (1.0f - expf(-k * stat_point)));
        }
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 130.0f;
            const float y100 = 350.0f;
            const float k = 0.1f;
            return (int)std::floor(stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k) + 0.5);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 130.0f;
            return (int)roundf(stat_exponential(stat_point, y0, yp));
        }
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatAd(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 50 + 2861.85 * (1 - e^(-0.004565 x))
            const float k = 0.004565f;
            const float B = 2861.85f;
            return (int)roundf(50.0f + B * (1.0f - expf(-k * stat_point)));
        }
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 50.0f;
            const float yp = 350.0f;
            const float y100 = 1060.0f;
            const float k = 0.1f;
            return (int)std::floor(stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k) + 0.5);
        }
        case EXPONENTIAL: {
            const float y0 = 50.0f;
            const float yp = 350.0f;
            return (int)roundf(stat_exponential(stat_point, y0, yp));
        }
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatArmor(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.00875 * x^2 + 7.325 * x + 80
            return (int)roundf(0.00875f * stat_point * stat_point
                 + 7.325f * stat_point
                 + 80.0f);
        }
        case PIECEWISE:
            return (int)roundf(80.0f + stat_point * 8.2f);
        case EXPONENTIAL:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatArmorPen(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.4005 * (1 - exp(-0.06915 * x))
            const float L = 0.4005f;
            const float k = 0.06915f;
            return (int)roundf(L * (1.0f - expf(-k * stat_point)));
        }
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 0.3f;
            const float y100 = 0.4f;
            const float k = 0.1f;
            return (int)std::floor(stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k) + 0.5);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.3f;
            return (int)roundf(stat_exponential(stat_point, y0, yp));
        }
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatLethality(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 110.76 * (1 - exp(-0.05 * x))
            const float L = 110.76f;
            const float k = 0.05f;
            return (int)roundf(L * (1.0f - expf(-k * stat_point)));
        }
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 70.0f;
            const float y100 = 110.0f;
            const float k = 0.1f;
            return (int)std::floor(stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k) + 0.5);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 70.0f;
            return (int)roundf(stat_exponential(stat_point, y0, yp));
        }
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatAs(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 150 + 450 * exp(-0.038664 * x)
            const float C = 30.0f;
            const float D = 100.0f;
            const float k = 0.048664f;
            return (int)roundf(C + D * expf(-k * stat_point));
        }
        case PIECEWISE:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case EXPONENTIAL:
            return (int)roundf(80.0f + stat_point * 25.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatCrit(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 100 * (1 - exp(-0.0693147 * x))
            const float k = 0.0693147f;
            return (1.0f - expf(-k * stat_point));
        }
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 0.75f;
            const float y100 = 1.0f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.75f;
            float ret = stat_exponential(stat_point, y0, yp);
            if (ret < 1.0f) {
                return ret;
            }
            return 1.0f;
        }
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatCritFactor(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 2.85 - 1.35 * exp(-0.0463733 * x)
            const float L = 2.85f;
            const float B = 1.35f;
            const float k = 0.0463733f;
            return L - B * expf(-k * stat_point);
        }
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 1.5f;
            const float yp = 2.32f;
            const float y100 = 2.84f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 1.5f;
            const float yp = 2.32f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

//check P(bleed) > 1 --> bleed
float getStatBleed(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.02252 * x^0.8239
            const float B = 0.02252f;
            const float p = 0.8239f;
            return B * powf(stat_point, p);
        }
        case PIECEWISE: {
            const int p = 12;
            const float y0 = 0.0f;
            const float yp = 0.35f;
            const float y100 = 1.0f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.35f;
            float ret = stat_exponential(stat_point, y0, yp);
            if (ret < 1.0f) {
                return ret;
            }
            return 1.0f;
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

int getStatBleedDmg(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.05 + 0.0182 * x^0.569
            const float A = 0.05f;
            const float B = 0.0182f;
            const float p = 0.569f;
            return (int)roundf(A + B * powf(stat_point, p));
        }
        case PIECEWISE: {
            const int p = 12;
            const float y0 = 0.05f;
            const float yp = 0.15f;
            const float y100 = 0.3f;
            const float k = 0.1f;
            return (int)std::floor(stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k) + 0.5);
        }
        case EXPONENTIAL: {
            const float y0 = 0.05f;
            const float yp = 0.15f;
            return (int)roundf(stat_exponential(stat_point, y0, yp));
        }
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatBleedTicks(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 120 + 8.35 * x^0.829
            const float A = 120.0f;
            const float B = 8.35f;
            const float p = 0.829f;
            return (int)roundf(A + B * powf(stat_point, p));
        }
        case PIECEWISE: {
            const int p = 12;
            const float y0 = 1.2f;
            const float yp = 2.2f;
            const float y100 = 5.0f;
            const float k = 0.1f;
            return (int)std::floor(stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k) + 0.5);
        }
        case EXPONENTIAL: {
            const float y0 = 1.2f;
            const float yp = 2.2f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatAp(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 1597.9 * (1 - exp(-0.02354 * x))
            const float L = 1597.9f;
            const float k = 0.02354f;
            return (int)roundf(L * (1.0f - expf(-k * stat_point)));
        }
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 600.0f;
            const float y100 = 1450.0f;
            const float k = 0.1f;
            return (int)std::floor(stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k) + 0.5);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 600.0f;
            return (int)roundf(stat_exponential(stat_point, y0, yp));
        }
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatMr(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 43 + 12.59 * x^0.7266
            const float A = 43.0f;
            const float B = 12.59f;
            const float p = 0.7266f;
            return (int)roundf(A + B * powf(stat_point, p));
        }
        case PIECEWISE:
            return (int)roundf(43.0f + stat_point * 3.6f);
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 154.0f;
            return (int)roundf(stat_exponential(stat_point, y0, yp));
        }
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatMrPen(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.4005 * (1 - exp(-0.06915 * x))
            const float L = 0.4005f;
            const float k = 0.06915f;
            return (int)roundf(L * (1.0f - expf(-k * stat_point)));
        }
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 0.3f;
            const float y100 = 0.4f;
            const float k = 0.1f;
            return (int)std::floor(stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k) + 0.5);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.3f;
            return (int)roundf(stat_exponential(stat_point, y0, yp));
        }
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatEthereal(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 10.79 * x^0.22185
            const float B = 10.79f;
            const float p = 0.22185f;
            return (int)roundf(B * powf(stat_point, p));
        }
        case PIECEWISE:
        {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 21.0f;
            const float y100 = 30.0f;
            const float k = 0.1f;
            return (int)std::floor(stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k) + 0.5);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 21.0f;
            return (int)roundf(stat_exponential(stat_point, y0, yp));
        }
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

int getStatAh(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 14.25 * x^0.4804
            const float B = 14.25f;
            const float p = 0.4804f;
            return (int)roundf(B * powf(stat_point, p));
        }
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 60.0f;
            const float y100 = 130.0f;
            const float k = 0.1f;
            return (int)std::floor(stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k) + 0.5);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 60.0f;
            return (int)roundf(stat_exponential(stat_point, y0, yp));
        }
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatSmite(int stat_point) {
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
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 0.79f;
            const float y100 = 2.99f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.79f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

int getStatCdSmite(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x)=150+550 e^(-0.0313 x)
            const float C = 150.0f;
            const float D = 550.0f;
            const float k = 0.0313f;
            return (int)roundf(C + D * expf(-k * stat_point));
        }
        case PIECEWISE:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case EXPONENTIAL:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatBlast(int stat_point) {
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
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 0.26f;
            const float y100 = 1.0f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.26f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

int getStatCdBlast(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 500 + 700 * exp(-0.0693 * x)
            const float C = 400.0f;
            const float D = 800.0f;
            const float k = 0.0393f;
            return (int)roundf(C + D * expf(-k * stat_point));
        }
        case PIECEWISE:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case EXPONENTIAL:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatHeal(int stat_point) {
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
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 0.8f;
            const float y100 = 3.0f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.8f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

int getStatCdHeal(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 150 + 450 * exp(-0.038664 * x)
            const float C = 150.0f;
            const float D = 450.0f;
            const float k = 0.038664f;
            return (int)roundf(C + D * expf(-k * stat_point));
        }
        case PIECEWISE:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case EXPONENTIAL:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatStun(int stat_point) {
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
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 0.52f;
            const float y100 = 2.0f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.52f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

int getStatCdStun(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 300 + 700 * exp(-0.038664 * x)
            const float C = 300.0f;
            const float D = 700.0f;
            const float k = 0.038664f;
            return (int)roundf(C + D * expf(-k * stat_point));
        }
        case PIECEWISE:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case EXPONENTIAL:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatAcc(int stat_point) {
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
        case PIECEWISE: {
            const int p = 12;
            const float y0 = 0.0f;
            const float yp = 0.85f;
            const float y100 = 1.0f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.85f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

//depende del ap
float getStatAccTicks(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.8 + 0.117 * x^0.777
            const float A = 0.8f;
            const float B = 0.117f;
            const float p = 0.777f;
            return A + B * powf(stat_point, p);
        }
        case PIECEWISE: {
            const int p = 12;
            const float y0 = 0.8f;
            const float yp = 2.0f;
            const float y100 = 5.0f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k); 
        }
        case EXPONENTIAL: {
            const float y0 = 0.8f;
            const float yp = 2.0f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

int getStatCdAcc(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 300 + 300 * exp(-0.038664 * x)
            const float C = 300.0f;
            const float D = 300.0f;
            const float k = 0.038664f;
            return (int)roundf(C + D * expf(-k * stat_point));
        }
        case PIECEWISE:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case EXPONENTIAL:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatSlow(int stat_point) {
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
        case PIECEWISE: {
            const int p = 12;
            const float y0 = 0.0f;
            const float yp = 0.85f;
            const float y100 = 1.0f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.85f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

//depende del ap
float getStatSlowTicks(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.8 + 0.117 * x^0.777
            const float A = 0.8f;
            const float B = 0.117f;
            const float p = 0.777f;
            return A + B * powf(stat_point, p);
        }
        case PIECEWISE: {
            const int p = 12;
            const float y0 = 0.8f;
            const float yp = 2.0f;
            const float y100 = 5.0f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.8f;
            const float yp = 2.0f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

int getStatCdSlow(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 300 + 300 * exp(-0.038664 * x)
            const float C = 300.0f;
            const float D = 300.0f;
            const float k = 0.038664f;
            return (int)roundf(C + D * expf(-k * stat_point));
        }
        case PIECEWISE:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case EXPONENTIAL:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatShield(int stat_point) {
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
        case PIECEWISE: {
            const int p = 12;
            const float y0 = 0.0f;
            const float yp = 0.85f;
            const float y100 = 1.0f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.85f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

//depende del ap
float getStatShieldTicks(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.8 + 0.117 * x^0.777
            const float A = 0.8f;
            const float B = 0.117f;
            const float p = 0.777f;
            return A + B * powf(stat_point, p);
        }
        case PIECEWISE: {
            const int p = 12;
            const float y0 = 0.8f;
            const float yp = 2.0f;
            const float y100 = 5.0f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.8f;
            const float yp = 2.0f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

int getStatCdShield(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 100 + 300 * exp(-0.038664 * x)
            const float C = 100.0f;
            const float D = 300.0f;
            const float k = 0.038664f;
            return (int)roundf(C + D * expf(-k * stat_point));
        }
        case PIECEWISE:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case EXPONENTIAL:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatMark(int stat_point) {
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
        case PIECEWISE: {
            const int p = 12;
            const float y0 = 0.0f;
            const float yp = 0.85f;
            const float y100 = 1.0f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.85f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

//depende del ap
float getStatMarkTicks(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.8 + 0.117 * x^0.777
            const float A = 0.8f;
            const float B = 0.117f;
            const float p = 0.777f;
            return A + B * powf(stat_point, p);
        }
        case PIECEWISE: {
            const int p = 12;
            const float y0 = 0.8f;
            const float yp = 2.0f;
            const float y100 = 5.0f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.8f;
            const float yp = 2.0f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

int getStatCdMark(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 100 + 300 * exp(-0.038664 * x)
            const float C = 100.0f;
            const float D = 300.0f;
            const float k = 0.038664f;
            return (int)roundf(C + D * expf(-k * stat_point));
        }
        case PIECEWISE:
            return (int)roundf(80.0f + stat_point * 25.0f);
        case EXPONENTIAL:
            return (int)roundf(120.0f + stat_point * 15.0f);
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatVamp(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.01678 * x^0.8277
            const float B = 0.01678f;
            const float p = 0.8277f;
            return B * powf(stat_point, p);
        }
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 0.2f;
            const float y100 = 0.76f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.2f;
            float ret = stat_exponential(stat_point, y0, yp);
            if (ret < 1.0f) {
                return ret;
            }
            return 1.0f;
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

float getStatThorns(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.01678 * x^0.8277
            const float B = 0.01678f;
            const float p = 0.8277f;
            return B * powf(stat_point, p);
        }
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 0.2f;
            const float y100 = 0.76f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.2f;
            float ret = stat_exponential(stat_point, y0, yp);
            if (ret < 1.0f) {
                return ret;
            }
            return 1.0f;
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

int getStatAx(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 21.33 * x^0.847
            const float B = 21.33f;
            const float p = 0.847f;
            return (int)roundf(B * powf(stat_point, p));
        }
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 270.0f;
            const float y100 = 1054.0f;
            const float k = 0.1f;
            return (int)std::floor(stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k) + 0.5);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 270.0f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return (int)roundf(100.0f + stat_point * 20.0f);
    }
}

float getStatTenacity(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = 0.0971 * x^0.4307
            const float B = 0.0971f;
            const float p = 0.4307f;
            return B * powf(stat_point, p);
        }
        case PIECEWISE: {
            const int p = 20;
            const float y0 = 0.0f;
            const float yp = 0.35f;
            const float y100 = 0.7f;
            const float k = 0.1f;
            return stat_piecewise_exp_linear(stat_point, p, y0, yp, y100, k);
        }
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.35f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

int getStatAggro(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = x
			return stat_point;
        }
        case PIECEWISE:
            return stat_point;
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.5f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}

int getStatFocus(int stat_point) {
    switch (instance) {
        case BALANCED: {
            // f(x) = x
			return stat_point;
        }
        case PIECEWISE:
            return stat_point;
        case EXPONENTIAL: {
            const float y0 = 0.0f;
            const float yp = 0.5f;
            return stat_exponential(stat_point, y0, yp);
        }
        default:
            return 100.0f + stat_point * 20.0f;
    }
}
// TODO: Implement other stat functions similarly
