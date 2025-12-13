package org.evol;

import org.uma.jmetal.operator.mutation.MutationOperator;
import org.uma.jmetal.solution.integersolution.IntegerSolution;

import java.util.Random;

public class FlipMutation implements MutationOperator<IntegerSolution> {

    private final double mutationRate;
    private final Random random = new Random();

    public FlipMutation(double mutationRate) {
        this.mutationRate = mutationRate;
    }

    @Override
    public IntegerSolution execute(IntegerSolution solution) {

        for (int i = 0; i < RPGProblem.GENOME_SIZE; i++) {
            if (random.nextDouble() < mutationRate) {
                int newVal = random.nextInt(RPGProblem.STATS_PER_PLAYER);
                solution.variables().set(i, newVal);
            }
        }

        return solution;
    }

    @Override
    public double mutationProbability() {
        return mutationRate;
    }
}
