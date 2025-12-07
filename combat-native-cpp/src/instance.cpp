#include "instance.hpp"

float getStatMaxHp(int stat_point, Instance instance) {
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

// TODO: Implement other stat functions similarly