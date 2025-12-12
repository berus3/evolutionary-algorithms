package org.evol;

import org.uma.jmetal.problem.integerproblem.impl.AbstractIntegerProblem;
import org.uma.jmetal.solution.integersolution.IntegerSolution;

import java.util.ArrayList;
import java.util.List;

/*
 Representación del problema:
 Cada equipo tiene 5 jugadores y cada jugador realiza 100 elecciones:
 elegir una stat en el rango [0,42].
  Esto garantiza que cada jugador siempre asigna exactamente 100 puntos.
 */
 
public class RPGProblem extends AbstractIntegerProblem {

    public static final int STATS_PER_PLAYER = 43;
    public static final int DECISIONS_PER_PLAYER = 100;
    public static final int PLAYERS_PER_TEAM = 5;

    public static final int GENOME_SIZE =
            PLAYERS_PER_TEAM * DECISIONS_PER_PLAYER;   // 5*100 = 500 genes

    public RPGProblem() {
        setNumberOfVariables(GENOME_SIZE);
        setNumberOfObjectives(1);
        setName("RPGProblem");

        // Definir los límites: cada decisión es elegir una stat entre [0, 42]
        List<Integer> lower = new ArrayList<>(GENOME_SIZE);
        List<Integer> upper = new ArrayList<>(GENOME_SIZE);

        for (int i = 0; i < GENOME_SIZE; i++) {
            lower.add(0);
            upper.add(STATS_PER_PLAYER - 1);
        }

        setLowerLimit(lower);
        setUpperLimit(upper);
    }

    /* Evaluación diferida: el Evaluator por lotes se encarga del fitness */
    @Override
    public void evaluate(IntegerSolution solution) {
        // NO USAR. El fitness se asigna externamente usando JNI.
    }
}
