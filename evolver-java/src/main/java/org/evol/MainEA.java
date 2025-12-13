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
		//TODO: revisar los parametros (0.75, 0.002, y tama;o de población) para hacer una especie de grid search y ver cuales son los optiomos
        CrossoverOperator<IntegerSolution> crossover = new BlockUniformCrossover(0.75);

        MutationOperator<IntegerSolution> mutation = new FlipMutation(0.002);

        SelectionOperator<List<IntegerSolution>, IntegerSolution> selection = new BinaryTournamentSelection<>();

        ArenaEvaluator evaluator = new ArenaEvaluator();
        LoggerEA logger = new LoggerEA("logs/fitness.csv");

        final int popSize = 100;
        final int generations = 50;

        // initialize population as random factible solutions (?
        List<IntegerSolution> population = new ArrayList<>(popSize);
        for (int i = 0; i < popSize; i++) {
            population.add(problem.createSolution());
        }

        evaluator.evaluate(population, problem);
        logger.log(0, population);
        printGeneration(0, population);

        // evo loop
        for (int gen = 1; gen <= generations; gen++) {

            List<IntegerSolution> offspring = new ArrayList<>(popSize);

            while (offspring.size() < popSize) {
                IntegerSolution p1 = selection.execute(population);
                IntegerSolution p2 = selection.execute(population);

                List<IntegerSolution> children =
                        crossover.execute(List.of(p1, p2));

                mutation.execute(children.get(0));
                mutation.execute(children.get(1));

                offspring.addAll(children);
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

            // elitism
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

    // summarize gen
    private static void printGeneration(int gen, List<IntegerSolution> pop) {
        double best = pop.stream()
                .mapToDouble(s -> s.objectives()[0])
                .min()
                .orElse(Double.NaN);

        double worst = pop.stream()
                .mapToDouble(s -> s.objectives()[0])
                .max()
                .orElse(Double.NaN);

        long distinct = pop.stream()
                .map(s -> s.objectives()[0])
                .distinct()
                .count();

        System.out.println(
                String.format(
                        Locale.US,
                        "GEN %3d | best=%s worst=%s distinct=%d",
                        gen, f(best), f(worst), distinct
                )
        );
        System.out.println(); 
    }
}
