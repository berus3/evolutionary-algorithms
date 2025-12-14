package org.evol;

import org.uma.jmetal.operator.crossover.CrossoverOperator;
import org.uma.jmetal.operator.mutation.MutationOperator;
import org.uma.jmetal.operator.selection.SelectionOperator;
import org.uma.jmetal.operator.selection.impl.NaryTournamentSelection;
import org.uma.jmetal.solution.integersolution.IntegerSolution;
import org.uma.jmetal.util.comparator.ObjectiveComparator;
import org.uma.jmetal.util.pseudorandom.JMetalRandom;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Locale;

public class MainEA {

    private static final boolean DEBUG =
            Boolean.parseBoolean(System.getProperty("evol.debugEval", "false"));

    private static String f(double x) {
        return String.format(Locale.US, "%.4f", x);
    }

    // ============================================================
    // ENTRY POINT FOR GRID SEARCH
    // ============================================================
    public static void run(RunConfig cfg) {

        // ------------------------------------------------------------
        // RNG + instance
        // ------------------------------------------------------------
        JMetalRandom.getInstance().setSeed(cfg.seed);
        RPGNativeBridge.setSeed(cfg.seed);
        RPGNativeBridge.setInstance(cfg.instance.id);

        System.out.println("[RUN] " + cfg.tag());

        long startTimeNs = System.nanoTime();

        // ------------------------------------------------------------
        // Problem
        // ------------------------------------------------------------
        RPGProblem problem = new RPGProblem();

        // ------------------------------------------------------------
        // Operators
        // ------------------------------------------------------------
        CrossoverOperator<IntegerSolution> crossover =
                new BlockUniformCrossover(cfg.crossoverProb);

        int tournamentK = 3;
        SelectionOperator<List<IntegerSolution>, IntegerSolution> selection =
                new NaryTournamentSelection<>(
                        tournamentK,
                        new ObjectiveComparator<>(0)
                );

        // ------------------------------------------------------------
        // EA parameters
        // ------------------------------------------------------------
        final int popSize = cfg.popSize;
        final int generations = 100;
        final int elitismCount = 3;

        // mutation decay
        final double p0 = cfg.p0;
        final double pMin = cfg.pMin;
        final double alpha = cfg.alpha;

        // early stopping
        final int plateauWindow = 100;
        final double epsilon = 1e-4;

        // similarity
        final double lambdaSimilarity = 0.30;

        double bestEver = Double.POSITIVE_INFINITY;
        int noImprovementCount = 0;

        ArenaEvaluator evaluator = new ArenaEvaluator();
        evaluator.setLambdaSimilarity(lambdaSimilarity);

        // ------------------------------------------------------------
        // Logger (UNIQUE per config)
        // ------------------------------------------------------------
        LoggerEA logger = new LoggerEA(
                "logs/fitness_" + cfg.tag() + ".csv",
                cfg.seed,
                cfg.instance,
                popSize,
                generations,
                cfg.crossoverProb,
                p0,
                pMin,
                alpha,
                lambdaSimilarity,
                20,              // fights per team
                5,               // anchors
                tournamentK,
                elitismCount
        );

        // ------------------------------------------------------------
        // Hall of Fame
        // ------------------------------------------------------------
        HallOfFame hof = new HallOfFame(5);

        // ------------------------------------------------------------
        // Init population
        // ------------------------------------------------------------
        List<IntegerSolution> population = new ArrayList<>(popSize);
        for (int i = 0; i < popSize; i++) {
            population.add(problem.createSolution());
        }

        // ------------------------------------------------------------
        // Gen 0
        // ------------------------------------------------------------
        evaluator.evaluate(population, problem);
        insertBestIntoHoF(population, hof);

        logger.log(0, population);
        printGeneration(0, population);

        bestEver = population.stream()
                .mapToDouble(s -> s.objectives()[0])
                .min()
                .orElse(Double.POSITIVE_INFINITY);

        // ------------------------------------------------------------
        // Evolution loop
        // ------------------------------------------------------------
        for (int gen = 1; gen <= generations; gen++) {

            double pGen = Math.max(pMin, p0 * Math.pow(alpha, gen));
            MutationOperator<IntegerSolution> mutation =
                    new FlipMutation(pGen);

            List<IntegerSolution> offspring = new ArrayList<>(popSize);

            while (offspring.size() < popSize) {
                IntegerSolution p1 = selection.execute(population);
                IntegerSolution p2 = selection.execute(population);

                List<IntegerSolution> children =
                        crossover.execute(List.of(p1, p2));

                mutation.execute(children.get(0));
                mutation.execute(children.get(1));

                offspring.add(children.get(0));
                offspring.add(children.get(1));
            }

            evaluator.evaluate(offspring, problem);

            if (DEBUG) {
                double min = offspring.stream()
                        .mapToDouble(s -> s.objectives()[0])
                        .min().orElse(Double.NaN);

                double max = offspring.stream()
                        .mapToDouble(s -> s.objectives()[0])
                        .max().orElse(Double.NaN);

                long distinct = offspring.stream()
                        .map(s -> s.objectives()[0])
                        .distinct().count();

                System.out.println(
                        "OBJ | min=" + f(min) +
                        " max=" + f(max) +
                        " distinct=" + distinct
                );
            }

            // elitism (μ + λ)
            population.sort(Comparator.comparingDouble(a -> a.objectives()[0]));
            offspring.sort(Comparator.comparingDouble(a -> a.objectives()[0]));

            List<IntegerSolution> nextGen = new ArrayList<>(popSize);

            for (int i = 0; i < elitismCount; i++) {
                nextGen.add(population.get(i));
            }
            for (int i = 0; i < popSize - elitismCount; i++) {
                nextGen.add(offspring.get(i));
            }

            population = nextGen;

            // HoF update
            insertBestIntoHoF(population, hof);

            // early stopping
            double currentBest = population.get(0).objectives()[0];

            if (bestEver - currentBest > epsilon) {
                bestEver = currentBest;
                noImprovementCount = 0;
            } else {
                noImprovementCount++;
            }

            logger.log(gen, population);
            printGeneration(gen, population);

            if (noImprovementCount >= plateauWindow) {
                System.out.println(
                        "[EARLY STOP] plateau " + plateauWindow +
                        " at gen " + gen
                );
                break;
            }
        }

        // ------------------------------------------------------------
        // Finish
        // ------------------------------------------------------------
        long endTimeNs = System.nanoTime();
        double runtimeSeconds =
                (endTimeNs - startTimeNs) / 1_000_000_000.0;

        logger.writeTopIndividuals(population, 10);
        logger.writeHallOfFame(hof);
        logger.writeRuntime(runtimeSeconds);
        logger.close();
    }

    // ============================================================
    // Optional standalone run (debug / single run)
    // ============================================================
    public static void main(String[] args) {
        RunConfig cfg = new RunConfig(
                123456,
                100,
                0.05,
                0.002,
                0.90,
                0.75,
                RPGInstance.PIECEWISE
        );
        run(cfg);
    }

    // ============================================================
    // Helpers
    // ============================================================
    private static void insertBestIntoHoF(
            List<IntegerSolution> population,
            HallOfFame hof) {

        IntegerSolution best = population.stream()
                .min(Comparator.comparingDouble(s -> s.objectives()[0]))
                .orElse(null);

        if (best != null) {
            hof.tryInsert(best);
        }
    }

    private static void printGeneration(int gen, List<IntegerSolution> pop) {

        IntegerSolution best = pop.stream()
                .min(Comparator.comparingDouble(s -> s.objectives()[0]))
                .orElse(null);

        if (best == null) return;

        double fitness = -best.objectives()[0];
        double winrate = (double) best.attributes()
                .getOrDefault("winrate", Double.NaN);
        double similarity = (double) best.attributes()
                .getOrDefault("similarity", Double.NaN);

        System.out.printf(
                Locale.US,
                "GEN %3d | fitness=%6s | winrate=%6s | sim=%5s%n",
                gen,
                f(fitness),
                f(winrate),
                f(similarity)
        );
    }
}
