package org.evol;

import org.uma.jmetal.solution.integersolution.IntegerSolution;
import org.uma.jmetal.util.evaluator.SolutionListEvaluator;

import java.util.List;

public class ArenaEvaluator implements SolutionListEvaluator<IntegerSolution> {

    @Override
    public List<IntegerSolution> evaluate(List<IntegerSolution> population) {

        int popSize = population.size();
        int genomeSize = RPGProblem.GENOME_SIZE; // 500

        int[] flat = new int[popSize * genomeSize];

        // flatear toda la población
        for (int p = 0; p < popSize; p++) {
            IntegerSolution ind = population.get(p);
            for (int g = 0; g < genomeSize; g++) {
                flat[p * genomeSize + g] = ind.variables().get(g);
            }
        }

        // JNI: combate todos contra todos
        double[] winrates = RPGNativeBridge.evaluatePopulation(flat, popSize);

        // asignar fitness
        for (int i = 0; i < popSize; i++) {
            population.get(i).objectives()[0] = -winrates[i];
        }

        return population;
    }

    @Override
    public void shutdown() {
        // nada que limpiar
    }
}
