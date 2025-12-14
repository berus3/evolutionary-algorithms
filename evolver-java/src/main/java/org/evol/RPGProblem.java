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

    /** Devuelve el índice del jugador al que pertenece el gen */
    public static int playerOfGene(int geneIndex) {
        return geneIndex / DECISIONS_PER_PLAYER;
    }

    /** Devuelve el rango [start, end) de genes de un jugador */
    public static int playerBaseIndex(int player) {
        return player * DECISIONS_PER_PLAYER;
    }

    @Override
    public IntegerSolution evaluate(IntegerSolution solution) {
        // ArenaEvaluator asigns objectives
        return solution;
    }
}
