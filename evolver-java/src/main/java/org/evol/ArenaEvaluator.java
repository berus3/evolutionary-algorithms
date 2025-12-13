package org.evol;

import org.uma.jmetal.problem.Problem;
import org.uma.jmetal.solution.integersolution.IntegerSolution;
import org.uma.jmetal.util.evaluator.SolutionListEvaluator;

import java.util.List;
import java.util.Locale;

public class ArenaEvaluator implements SolutionListEvaluator<IntegerSolution> {

    private static final boolean DEBUG =
            Boolean.parseBoolean(System.getProperty("evol.debugEval", "false"));

    // printing format
    private static String f(double x) {
        return String.format(Locale.US, "%.3f", x);
    }

    @Override
    public List<IntegerSolution> evaluate(
            List<IntegerSolution> population,
            Problem<IntegerSolution> problem) {

        final int popSize = population.size();
        final int genomeSize = RPGProblem.GENOME_SIZE; // 500

        
        // flatten population
        int[] flat = new int[popSize * genomeSize];

        for (int p = 0; p < popSize; p++) {
            IntegerSolution ind = population.get(p);

            if (ind.variables().size() != genomeSize) {
                throw new IllegalStateException(
                        "Genome size mismatch. Expected " + genomeSize +
                        " but got " + ind.variables().size() + " at p=" + p);
            }

            int base = p * genomeSize;
            for (int g = 0; g < genomeSize; g++) {
                Integer v = ind.variables().get(g);
                flat[base + g] = (v == null) ? 0 : v;
            }
        }

        
         // JNI call
        double[] winrates = RPGNativeBridge.evaluatePopulation(flat, popSize);

        if (winrates == null) {
            throw new IllegalStateException("JNI returned null winrates");
        }
        if (winrates.length != popSize) {
            throw new IllegalStateException(
                    "JNI returned winrates.length=" + winrates.length +
                    " but popSize=" + popSize);
        }

        if (DEBUG) {
            System.out.print("WR sample: ");
            for (int i = 0; i < Math.min(10, winrates.length); i++) {
                System.out.print(f(winrates[i]) + " ");
            }
            System.out.println();
        }
        // assign objectives
        double min = Double.POSITIVE_INFINITY;
        double max = Double.NEGATIVE_INFINITY;
        int distinctish = 0;

        double prev = Double.NaN;

        for (int i = 0; i < popSize; i++) {
            double wr = winrates[i];

            if (!Double.isFinite(wr)) {
                throw new IllegalStateException(
                        "Non-finite winrate at i=" + i + ": " + wr);
            }

            // objective minimize ( - winrate)
            population.get(i).objectives()[0] = -wr;

            if (wr < min) min = wr;
            if (wr > max) max = wr;
            if (i == 0 || wr != prev) distinctish++;
            prev = wr;
        }

        if (DEBUG) {
            System.out.println(
                    "WINRATE | min=" + f(min) +
                    " max=" + f(max) +
                    " first=" + f(winrates[0]) +
                    " distinct=" + distinctish
            );
        }

        return population;
    }

    @Override
    public void shutdown() {
        // nothing
    }
}
