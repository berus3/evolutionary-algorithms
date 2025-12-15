package org.evol;

public class RPGNativeBridge {

    static {
        System.loadLibrary("combat"); // carga libcombat.so
    }

    // Firma generada por javac -h
    public static native double[] evaluatePopulation(int[] flatPopulation, int populationSize);
    
    public static native void setSeed(int seed);
    
    public static native void setInstance(int instanceId);
    
    public static native double[] evaluateReferences(
            int[] flatReferences,
            int refCount,
            int[] flatPopulation,
            int popCount,
            int[] flatHallOfFame,
            int hofCount,
            int fightsPerRef
    );
    
    
}
