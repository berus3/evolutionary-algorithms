package org.evol;

import org.uma.jmetal.operator.crossover.CrossoverOperator;
import org.uma.jmetal.solution.integersolution.IntegerSolution;

import java.util.List;
import org.uma.jmetal.util.pseudorandom.JMetalRandom;


public class BlockUniformCrossover
        implements CrossoverOperator<IntegerSolution> {

    private final double crossoverProbability;
    private final JMetalRandom rng = JMetalRandom.getInstance();

    public BlockUniformCrossover(double crossoverProbability) {
        this.crossoverProbability = crossoverProbability;
    }

    @Override
    public List<IntegerSolution> execute(List<IntegerSolution> parents) {
        IntegerSolution p1 = parents.get(0);
        IntegerSolution p2 = parents.get(1);

        IntegerSolution c1 = (IntegerSolution) p1.copy();
        IntegerSolution c2 = (IntegerSolution) p2.copy();

        if (rng.nextDouble() < crossoverProbability) {

            int players   = RPGProblem.PLAYERS_PER_TEAM;     // 5
            int blockSize = RPGProblem.DECISIONS_PER_PLAYER; // 100

            for (int player = 0; player < players; player++) {

                // coin flip por jugador
                if (rng.nextDouble() < 0.5) {

                    int start = player * blockSize;
                    int end   = start + blockSize;

                    for (int i = start; i < end; i++) {
                        int tmp = c1.variables().get(i);
                        c1.variables().set(i, c2.variables().get(i));
                        c2.variables().set(i, tmp);
                    }
                }
            }
        }

        return List.of(c1, c2);
    }

    @Override
    public double crossoverProbability() {
        return crossoverProbability;
    }

    @Override
    public int numberOfRequiredParents() {
        return 2;
    }

    @Override
    public int numberOfGeneratedChildren() {
        return 2;
    }
}
