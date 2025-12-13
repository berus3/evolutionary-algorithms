package org.evol;

import org.uma.jmetal.problem.integerproblem.impl.AbstractIntegerProblem;
import org.uma.jmetal.solution.integersolution.IntegerSolution;

import java.util.ArrayList;
import java.util.List;

public class RPGProblem extends AbstractIntegerProblem {

    public static final int STATS_PER_PLAYER = 43;
    public static final int DECISIONS_PER_PLAYER = 100;
    public static final int PLAYERS_PER_TEAM = 5;
    public static final int GENOME_SIZE =
            PLAYERS_PER_TEAM * DECISIONS_PER_PLAYER; // 500

    public RPGProblem() {
        numberOfObjectives(1);
        numberOfConstraints(0);
        name("RPGProblem");

        List<Integer> lower = new ArrayList<>(GENOME_SIZE);
        List<Integer> upper = new ArrayList<>(GENOME_SIZE);

        for (int i = 0; i < GENOME_SIZE; i++) {
            lower.add(0);
            upper.add(STATS_PER_PLAYER - 1);
        }

        variableBounds(lower, upper);
    }

    @Override
    public IntegerSolution evaluate(IntegerSolution solution) {
        // ArenaEvaluator asignes objectives
        return solution;
    }
}
