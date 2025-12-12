package org.evol;

public class RPGNativeBridge {

    static {
        System.loadLibrary("combat"); // carga libcombat.so
    }

    // Firma generada por javac -h
    public static native double[] evaluatePopulation(int[] flatPopulation, int populationSize);
}
