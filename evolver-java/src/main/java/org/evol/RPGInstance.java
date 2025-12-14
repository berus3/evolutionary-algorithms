package org.evol;

public enum RPGInstance {
    BALANCED(0),
    PIECEWISE(1),
    EXPONENTIAL(2),
    SUPPORT_LIKE_PIECEWISE(3),
    UNFAIR(4);

    public final int id;

    RPGInstance(int id) {
        this.id = id;
    }
}
