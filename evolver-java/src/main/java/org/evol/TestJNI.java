package org.evol;

import java.util.Arrays;

public class TestJNI {

    public static void main(String[] args) {

        int populationSize = 2;
        int genomeSize = 43 * 5; // 215

        int[] flat = new int[populationSize * genomeSize];

        for (int i = 0; i < flat.length; i++) {
            flat[i] = 1;
        }

        double[] result = RPGNativeBridge.evaluatePopulation(flat, populationSize);

        System.out.println("Winrates: " + Arrays.toString(result));
    }
}
