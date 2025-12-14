package org.evol;

public enum RPGInstance {
    BALANCED(0),
    PIECEWISE(1),
    EXPONENTIAL(2),
    SUPPORT_LIKE_PIECEWISE(3),
    FULL_DAMAGE_PIECEWISE(4),
    UNFAIR(5);

    public final int id;

    RPGInstance(int id) {
        this.id = id;
    }
}
