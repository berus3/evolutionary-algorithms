package org.evol;

import org.uma.jmetal.problem.Problem;
import org.uma.jmetal.solution.integersolution.IntegerSolution;
import org.uma.jmetal.util.evaluator.SolutionListEvaluator;

import java.util.*;
import java.util.stream.IntStream;

public class ArenaEvaluator implements SolutionListEvaluator<IntegerSolution> {

    private static final boolean DEBUG =
            Boolean.parseBoolean(System.getProperty("evol.debugEval", "false"));

    // local window for similarity
    private static final int SIM_WINDOW = 10;

    public double lambdaSimilarity;

    public void setLambdaSimilarity(double lambda) {
        if (lambda < 0.0)
            throw new IllegalArgumentException("lambda must be >= 0");
        this.lambdaSimilarity = lambda;
    }

    private static String f(double x) {
        return String.format(Locale.US, "%.3f", x);
    }

    @Override
    public List<IntegerSolution> evaluate(
            List<IntegerSolution> population,
            Problem<IntegerSolution> problem) {

        final int popSize = population.size();
        final int genomeSize = RPGProblem.GENOME_SIZE;

        // flatten population
        int[] flat = new int[popSize * genomeSize];

        for (int p = 0; p < popSize; p++) {
            IntegerSolution ind = population.get(p);

            if (ind.variables().size() != genomeSize) {
                throw new IllegalStateException("Genome size mismatch");
            }

            int base = p * genomeSize;
            for (int g = 0; g < genomeSize; g++) {
                Integer v = ind.variables().get(g);
                flat[base + g] = (v == null) ? 0 : v;
            }
        }

        // JNI evaluation -> winrates
        double[] winrates = RPGNativeBridge.evaluatePopulation(flat, popSize);
        if (winrates == null || winrates.length != popSize) {
            throw new IllegalStateException("JNI returned invalid winrates");
        }

        if (DEBUG) {
            System.out.print("WR sample: ");
            for (int i = 0; i < Math.min(10, popSize); i++) {
                System.out.print(f(winrates[i]) + " ");
            }
            System.out.println();
        }

        // buckets for cosine similarity
        int[][] buckets = new int[popSize][RPGProblem.STATS_PER_PLAYER];

        for (int i = 0; i < popSize; i++) {
            int base = i * genomeSize;
            for (int g = 0; g < genomeSize; g++) {
                int stat = flat[base + g];
                if (stat >= 0 && stat < RPGProblem.STATS_PER_PLAYER) {
                    buckets[i][stat]++;
                }
            }
        }

        // order by winrate (just for neighbours)
        int[] orderByWR = IntStream.range(0, popSize)
                .boxed()
                .sorted(Comparator.comparingDouble(i -> winrates[i]))
                .mapToInt(i -> i)
                .toArray();

        // local similarity for EVERYBODY
        double[] similarity = new double[popSize];

        for (int pos = 0; pos < popSize; pos++) {
            int i = orderByWR[pos];
            double sum = 0.0;
            int cnt = 0;

            for (int d = 1; d <= SIM_WINDOW; d++) {
                if (pos - d >= 0) {
                    int j = orderByWR[pos - d];
                    sum += cosineSimilarity(buckets[i], buckets[j]);
                    cnt++;
                }
                if (pos + d < popSize) {
                    int j = orderByWR[pos + d];
                    sum += cosineSimilarity(buckets[i], buckets[j]);
                    cnt++;
                }
            }

            similarity[i] = (cnt == 0) ? 0.0 : sum / cnt;
        }

        // final fitness = winrate − λ·similarity  (FOR EVERYBODY)
        double minWR = Double.POSITIVE_INFINITY;
        double maxWR = Double.NEGATIVE_INFINITY;

        for (int i = 0; i < popSize; i++) {
            double wr  = winrates[i];
            double sim = similarity[i];

            double fitness = wr - lambdaSimilarity * sim;

            population.get(i).objectives()[0] = -fitness;
            population.get(i).attributes().put("winrate", wr);
            population.get(i).attributes().put("similarity", sim);
            population.get(i).attributes().put("fitness", fitness);

            minWR = Math.min(minWR, wr);
            maxWR = Math.max(maxWR, wr);
        }

        if (DEBUG) {
            System.out.println(
                "FITNESS | wr[min=" + f(minWR) +
                ", max=" + f(maxWR) +
                "] λ=" + lambdaSimilarity +
                " SIM_WINDOW=" + SIM_WINDOW
            );
        }

        return population;
    }

    // cosine similarity
    private static double cosineSimilarity(int[] a, int[] b) {
        long dot = 0, na = 0, nb = 0;

        for (int i = 0; i < a.length; i++) {
            dot += (long) a[i] * b[i];
            na  += (long) a[i] * a[i];
            nb  += (long) b[i] * b[i];
        }

        if (na == 0 || nb == 0) return 0.0;
        return dot / (Math.sqrt((double) na) * Math.sqrt((double) nb));
    }

    @Override
    public void shutdown() {
        // do nothing
    }
}
