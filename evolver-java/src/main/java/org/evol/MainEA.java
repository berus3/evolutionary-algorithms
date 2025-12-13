package org.evol;

import org.uma.jmetal.operator.crossover.CrossoverOperator;
import org.uma.jmetal.operator.mutation.MutationOperator;
import org.uma.jmetal.operator.selection.SelectionOperator;
import org.uma.jmetal.operator.selection.impl.BinaryTournamentSelection;
import org.uma.jmetal.solution.integersolution.IntegerSolution;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

public class MainEA {

    private static final boolean DEBUG =
            Boolean.parseBoolean(System.getProperty("evol.debugEval", "false"));

    private static String f(double x) {
        return String.format(Locale.US, "%.4f", x);
    }

    public static void main(String[] args) {

        RPGProblem problem = new RPGProblem();

        // === Operators ===
        CrossoverOperator<IntegerSolution> crossover =
                new BlockUniformCrossover(0.75);

        SelectionOperator<List<IntegerSolution>, IntegerSolution> selection =
                new BinaryTournamentSelection<>();

        // === Mutation decay ===
        double p0    = 0.10;
        double pMin  = 0.002;
        double alpha = 0.90;

        ArenaEvaluator evaluator = new ArenaEvaluator();
        LoggerEA logger = new LoggerEA("logs/fitness.csv");

        final int popSize = 100;
        final int generations = 50;

        // === Init population ===
        List<IntegerSolution> population = new ArrayList<>(popSize);
        for (int i = 0; i < popSize; i++) {
            population.add(problem.createSolution());
        }

        evaluator.evaluate(population, problem);
        logger.log(0, population);
        printGeneration(0, population);

        // === Evolution loop ===
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
                        "OBJ   | min=" + f(min) +
                        " max=" + f(max) +
                        " distinct=" + distinct
                );
            }

            // === Elitist (μ + λ) replacement ===
            population.sort((a, b) ->
                    Double.compare(a.objectives()[0], b.objectives()[0]));
            offspring.sort((a, b) ->
                    Double.compare(a.objectives()[0], b.objectives()[0]));

            List<IntegerSolution> nextGen = new ArrayList<>(popSize);
            nextGen.add(population.get(0));
            nextGen.add(population.get(1));

            for (int i = 0; i < popSize - 2; i++) {
                nextGen.add(offspring.get(i));
            }

            population = nextGen;

            logger.log(gen, population);
            printGeneration(gen, population);
        }

        logger.close();
    }

    // === Pretty generation summary ===
    private static void printGeneration(int gen, List<IntegerSolution> pop) {

        IntegerSolution best = pop.stream()
                .min((a, b) -> Double.compare(a.objectives()[0], b.objectives()[0]))
                .orElse(null);

        if (best == null) return;

        double fitness = -best.objectives()[0];
        double winrate = (double) best.attributes()
                .getOrDefault("winrate", Double.NaN);
        double similarity = (double) best.attributes()
                .getOrDefault("similarity", Double.NaN);

        System.out.printf(
                Locale.US,
                "GEN %3d | fitness=%6s | winrate=%6s | sim=%5s%n%n",
                gen,
                f(fitness),
                f(winrate),
                f(similarity)
        );
    }
}
