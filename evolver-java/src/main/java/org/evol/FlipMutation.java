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
            {23, 24},            // stun, cd_stun
            {31, 32, 33},        // shield, shield_ticks, cd_shield
            {17, 18},            // smite, cd_smite
            {19, 20},            // blast, cd_blast
            {12, 16},            // ap, ah
            {5, 4},            // lethality, armor_pen
            {15,14},            // ethereal, mr_pen
            {2, 6, 37},            // ad, as, vamp
            {25, 26, 27},        // acc, acc_ticks, cd_acc
            {28, 29, 30},        // slow, slow_ticks, cd_slow
            {34, 35, 36},        // mark, mark_ticks, cd_mark
            {0,1,3,13, 38},        // max_hp, regen, mr, armor
            {39}, 				// ax
            {40}, 				// tenacity
            
   
    };

    // Dependencias DIRIGIDAS (asimétricas)
    // Formato: { stat_dependiente, soporte1, soporte2, ... }
    private static final int[][] DIRECTED_DEPENDENCIES = {
            {9, 2},      // bleed -> ad
            {10, 2},     // bleed_dmg -> ad
            {11, 2},     // bleed_ticks -> ad

            {4, 2},          // armor_pen -> ad
            {0, 41},          // max_hp -> aggro
            
            {7, 2},          // crit_ -> ad
            
            {5, 2},          // lethality -> ad

            {14, 12},        // mr_pen -> ap
            {15, 12},        // ethereal -> ap

            {21, 12},    // heal -> ap
            {22, 21},    // cd_heal -> heal 

            {25, 12},    // acc -> ap
			{27, 25},   // cd_ac -> acc
			{26, 25},   // cd_ac -> acc
			

		
            {28, 12},    // slow -> ap
			{30, 28},    // cd_slow -> slow
			{29, 28},    // slow ticks -> slow

            {31, 12},    // shield -> ap
            {33, 31},    // cd_shield -> shield
            {32, 31},    // shield ticks -> shield
		
            {34, 12},    // mark -> ap
            {36, 34},    // cd_mark -> mark
            {35, 34},    // mark ticks -> mark
		
            
            {23, 12},    // stun -> ap
            {24, 23},    // cd_stun -> stun
            
            {19, 12},    // blast -> ap
            {20, 19},    // cd_blast -> blast
            
            {17, 12},    // smite -> ap
            {18, 17},    // cd_smite -> smite

            {41, 0, 3},      // aggro -> max_hp, armor
            {2, 42},     // ad -> focus
            {12, 42},     // ap -> focus
    };

    // intensity
    private static final double P_GROUP_REINFORCE = 0.6f;
    private static final double P_DEP_REINFORCE   = 0.6f;

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
