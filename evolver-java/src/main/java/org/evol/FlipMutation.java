package org.evol;

import org.uma.jmetal.operator.mutation.MutationOperator;
import org.uma.jmetal.solution.integersolution.IntegerSolution;
import org.uma.jmetal.util.pseudorandom.JMetalRandom;

public class FlipMutation implements MutationOperator<IntegerSolution> {

    private final double mutationRate;
    private final JMetalRandom rng = JMetalRandom.getInstance();

    // epistasis definitions

    // Grupos epistáticos SIMÉTRICOS (building blocks)
    private static final int[][] EPISTATIC_GROUPS = {
            {7, 8},              // crit, crit_factor
            {9, 10, 11},         // bleed, bleed_dmg, bleed_ticks
            {21, 22},            // heal, cd_heal
            {31, 32, 33},        // shield, shield_ticks, cd_shield
            {17, 18},            // smite, cd_smite
            {19, 20},            // blast, cd_blast
            {25, 26, 27},        // acc, acc_ticks, cd_acc
            {28, 29, 30},        // slow, slow_ticks, cd_slow
            {34, 35, 36},        // mark, mark_ticks, cd_mark
    };

    // Dependencias DIRIGIDAS (asimétricas)
    // Formato: { stat_dependiente, soporte1, soporte2, ... }
    private static final int[][] DIRECTED_DEPENDENCIES = {
            {9, 2, 39},      // bleed -> ad, ax
            {10, 2, 39},     // bleed_dmg -> ad, ax
            {11, 2, 39},     // bleed_ticks -> ad, ax

            {4, 2},          // armor_pen -> ad
            
            {5, 2},          // lethality -> ad

            {14, 12},        // mr_pen -> ap
            {15, 12},        // ethereal -> ap

            {21, 12, 39},    // heal -> ap, ax
            {22, 12, 39},    // cd_heal -> ap, ax

            {25, 12, 39},    // acc -> ap, ax
            {26, 12, 39},
            {27, 12, 39},

            {28, 12, 39},    // slow -> ap, ax
            {29, 12, 39},
            {30, 12, 39},

            {31, 12, 39},    // shield -> ap, ax
            {32, 12, 39},
            {33, 12, 39},

            {34, 12, 39},    // mark -> ap, ax
            {35, 12, 39},
            {36, 12, 39},
            
            {23, 12, 39},    // stun -> ap, ax
            {24, 12, 39},
            
            {19, 12, 39},    // blast -> ap, ax
            {20, 12, 39},
            
            {17, 12, 39},    // smite -> ap, ax
            {18, 12, 39},

            {41, 0, 3},      // aggro -> max_hp, armor
            {42, 2, 12},     // focus -> ad, ap (soft)
    };

    // intensity
    private static final double P_GROUP_REINFORCE = 0.15;
    private static final double P_DEP_REINFORCE   = 0.15;

    public FlipMutation(double mutationRate) {
        this.mutationRate = mutationRate;
    }

    @Override
    public IntegerSolution execute(IntegerSolution solution) {

        for (int gene = 0; gene < RPGProblem.GENOME_SIZE; gene++) {

            if (rng.nextDouble() >= mutationRate)
                continue;

            int newStat = rng.nextInt(0, RPGProblem.STATS_PER_PLAYER - 1);
            solution.variables().set(gene, newStat);

            int player = gene / RPGProblem.DECISIONS_PER_PLAYER;
            int base = player * RPGProblem.DECISIONS_PER_PLAYER;

            // 1) Epistasis simétrica (building blocks)
            int[] group = findEpistaticGroup(newStat);
            if (group != null) {
                reinforce(solution, base, group, P_GROUP_REINFORCE);
            }

            // 2) Epistasis dirigida (dependencias)
            int[] deps = findDirectedDependencies(newStat);
            if (deps != null) {
                reinforce(solution, base, deps, P_DEP_REINFORCE);
            }
        }

        return solution;
    }

    // helpers

    private static int[] findEpistaticGroup(int stat) {
        for (int[] g : EPISTATIC_GROUPS) {
            for (int s : g) {
                if (s == stat)
                    return g;
            }
        }
        return null;
    }

    private static int[] findDirectedDependencies(int stat) {
        for (int[] d : DIRECTED_DEPENDENCIES) {
            if (d[0] == stat) {
                int[] deps = new int[d.length - 1];
                System.arraycopy(d, 1, deps, 0, deps.length);
                return deps;
            }
        }
        return null;
    }

    private void reinforce(IntegerSolution sol, int base, int[] stats, double p) {
        for (int g = base; g < base + RPGProblem.DECISIONS_PER_PLAYER; g++) {
            if (rng.nextDouble() < p) {
                sol.variables().set(
                        g,
                        stats[rng.nextInt(0, stats.length - 1)]
                );
            }
        }
    }

    @Override
    public double mutationProbability() {
        return mutationRate;
    }
}
