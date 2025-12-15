package org.evol;

import java.util.List;

public class EvaluationRunner {

    private static final List<Integer> SEEDS = List.of(
            31,32,33,34,35,36,37,38,39,40,
            41,42,43,44,45,46,47,48,49,50,
            51,52,53,54,55,56,57,58,59,60
    );

    private static final List<RPGInstance> INSTANCES = List.of(
            RPGInstance.SUPPORT_LIKE_PIECEWISE,
            RPGInstance.FULL_DAMAGE_PIECEWISE,
            RPGInstance.UNFAIR
    );

    private static final int FIGHTS_PER_REFERENCE = 10;

    public static void main(String[] args) {

        for (RPGInstance instance : INSTANCES) {
            System.out.println("\n=== INSTANCE " + instance + " ===");

            for (int seed : SEEDS) {

                RPGNativeBridge.setSeed(seed);
                RPGNativeBridge.setInstance(instance.id);

                int[] refGenomes = loadReferenceGenomes();
                int refCount = refGenomes.length / RPGProblem.GENOME_SIZE;

                int[] popGenomes = loadFinalPopulation(seed, instance);
                int popCount = popGenomes.length / RPGProblem.GENOME_SIZE;

                int[] hofGenomes = loadHallOfFame(seed, instance);
                int hofCount = hofGenomes.length / RPGProblem.GENOME_SIZE;

                double[] refWinrates =
                        RPGNativeBridge.evaluateReferences(
                                refGenomes,
                                refCount,
                                popGenomes,
                                popCount,
                                hofGenomes,
                                hofCount,
                                FIGHTS_PER_REFERENCE
                        );

                double bestAeFitness =
                        loadBestAeFitness(seed, instance);

                report(seed, instance, refWinrates, bestAeFitness);
            }
        }
    }

    // =========================================================
    // PLACEHOLDERS (los implementás leyendo tus CSV)
    // =========================================================

    private static int[] loadReferenceGenomes() {
        throw new UnsupportedOperationException();
    }

    private static int[] loadFinalPopulation(int seed, RPGInstance inst) {
        throw new UnsupportedOperationException();
    }

    private static int[] loadHallOfFame(int seed, RPGInstance inst) {
        return new int[0];
    }

    private static double loadBestAeFitness(int seed, RPGInstance inst) {
        throw new UnsupportedOperationException();
    }

    private static void report(
            int seed,
            RPGInstance inst,
            double[] refWinrates,
            double aeFitness
    ) {
        System.out.printf(
                "seed=%d | inst=%s | AE=%+.4f | refs=",
                seed, inst.name(), aeFitness
        );
        for (double w : refWinrates)
            System.out.printf("%.4f ", w);
        System.out.println();
    }
}
