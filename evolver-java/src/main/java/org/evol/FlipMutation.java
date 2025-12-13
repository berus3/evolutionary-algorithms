package org.evol;

import org.uma.jmetal.operator.mutation.MutationOperator;
import org.uma.jmetal.solution.integersolution.IntegerSolution;

import org.uma.jmetal.util.pseudorandom.JMetalRandom;


public class FlipMutation implements MutationOperator<IntegerSolution> {

    private final double mutationRate;
    private final JMetalRandom rng = JMetalRandom.getInstance();

    public FlipMutation(double mutationRate) {
        this.mutationRate = mutationRate;
    }

    @Override
    public IntegerSolution execute(IntegerSolution solution) {

        for (int i = 0; i < RPGProblem.GENOME_SIZE; i++) {
            if (rng.nextDouble() < mutationRate) {
                int newVal = rng.nextInt(0, RPGProblem.STATS_PER_PLAYER - 1);
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
