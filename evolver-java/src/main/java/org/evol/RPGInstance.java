package org.evol;

public enum RPGInstance {
    BALANCED(0),
    PIECEWISE(1),
    EXPONENTIAL(2),
    UNFAIR(3);

    public final int id;

    RPGInstance(int id) {
        this.id = id;
    }
}
