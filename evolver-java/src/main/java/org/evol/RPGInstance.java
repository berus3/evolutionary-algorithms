package org.evol;

public enum RPGInstance {
    BALANCED(0),
    LINEAR(1),
    UNFAIR(2);

    public final int id;

    RPGInstance(int id) {
        this.id = id;
    }
}
