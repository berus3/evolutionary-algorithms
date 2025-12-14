package org.evol;

import org.uma.jmetal.operator.crossover.CrossoverOperator;
import org.uma.jmetal.operator.mutation.MutationOperator;
import org.uma.jmetal.operator.selection.SelectionOperator;
import org.uma.jmetal.operator.selection.impl.NaryTournamentSelection;
import org.uma.jmetal.solution.integersolution.IntegerSolution;
import org.uma.jmetal.util.comparator.ObjectiveComparator;
import org.uma.jmetal.util.pseudorandom.JMetalRandom;

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

        // RNG seed
        int seed = 123456;

        JMetalRandom.getInstance().setSeed(seed);
        RPGNativeBridge.setSeed(seed);

        System.out.println("[SEED] " + seed);

        // instance
        RPGInstance instance = RPGInstance.PIECEWISE;

        RPGNativeBridge.setInstance(instance.id);

        System.out.println("[INSTANCE] " + instance);

        // START TIMER
        long startTimeNs = System.nanoTime();

        RPGProblem problem = new RPGProblem();

        // operators
        
        double crossoverProb = 0.75;
        CrossoverOperator<IntegerSolution> crossover =
                new BlockUniformCrossover(crossoverProb);

        int tournamentK = 3;
        SelectionOperator<List<IntegerSolution>, IntegerSolution> selection =
                new NaryTournamentSelection<>(
                        tournamentK,
                        new ObjectiveComparator<>(0)
                );

        // mutation decay

        double p0    = 0.30;
        double pMin  = 0.02;
        double alpha = 0.90;

        // EA parameters

        final int popSize = 100;
        final int generations = 100;
        final int elitismCount = 3;

        // early stopping (plateau)
       
        final int plateauWindow = 20;
        final double epsilon = 1e-4;
        
        // lambda (for similarity)
        
        double lambdaSimilarity = 0.30;

        double bestEver = Double.POSITIVE_INFINITY;
        int noImprovementCount = 0;

        ArenaEvaluator evaluator = new ArenaEvaluator();
        evaluator.setLambdaSimilarity(lambdaSimilarity);

        LoggerEA logger = new LoggerEA(
                "logs/fitness.csv",
    -            seed,
                instance,
                popSize,
                generations,
                crossoverProb,
                p0,
                pMin,
                alpha,
                lambdaSimilarity,
                5,              // fights per team
                5,              // anchors
                tournamentK,
                elitismCount
        );

        // init population
     
        List<IntegerSolution> population = new ArrayList<>(popSize);
        for (int i = 0; i < popSize; i++) {
            population.add(problem.createSolution());
        }

        evaluator.evaluate(population, problem);
        logger.log(0, population);
        printGeneration(0, population);

        bestEver = population.stream()
                .mapToDouble(s -> s.objectives()[0])
                .min().orElse(Double.POSITIVE_INFINITY);

        // evolution loop
       
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

            // elitism (mu + lambda)
            
            population.sort((a, b) ->
                    Double.compare(a.objectives()[0], b.objectives()[0]));
            offspring.sort((a, b) ->
                    Double.compare(a.objectives()[0], b.objectives()[0]));

            List<IntegerSolution> nextGen = new ArrayList<>(popSize);

            for (int i = 0; i < elitismCount; i++) {
                nextGen.add(population.get(i));
            }

            for (int i = 0; i < popSize - elitismCount; i++) {
                nextGen.add(offspring.get(i));
            }

            population = nextGen;

            // early stopping check
           
            double currentBest = population.get(0).objectives()[0];

            if (bestEver - currentBest > epsilon) {
                bestEver = currentBest;
                noImprovementCount = 0;
            } else {
                noImprovementCount++;
            }

            if (noImprovementCount >= plateauWindow) {
                System.out.println(
                        "[EARLY STOP] No significant improvement for " +
                        plateauWindow + " generations. Stopping at gen " + gen
                );
                logger.log(gen, population);
                printGeneration(gen, population);
                break;
            }

            logger.log(gen, population);
            printGeneration(gen, population);
        }

        // END TIMER
       
        long endTimeNs = System.nanoTime();
        double runtimeSeconds =
                (endTimeNs - startTimeNs) / 1_000_000_000.0;

        // final TOP-10 summary
       
        logger.writeTopIndividuals(population, 10);

        logger.writeRuntime(runtimeSeconds);
        logger.close();
    }

    // pretty print generation

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
