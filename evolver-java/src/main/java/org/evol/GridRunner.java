package org.evol;

import java.util.List;

public class GridRunner {

    public static void main(String[] args) {

        List<Integer> seeds = List.of(31, 32, 33, 34 ,35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60);
        List<Integer> popSizes = List.of(100);
        List<Double> crossProbs = List.of(0.25);
        List<Double> p0s = List.of(0.05);
        List<RPGInstance> instances = List.of(
				RPGInstance.SUPPORT_LIKE_PIECEWISE,
                RPGInstance.FULL_DAMAGE_PIECEWISE,
                RPGInstance.LINEAR
        );

        for (int seed : seeds)
        for (int pop : popSizes)
        for (double cross : crossProbs)
        for (double p0 : p0s)
        for (RPGInstance inst : instances) {

            RunConfig cfg = new RunConfig(
                    seed,
                    pop,
                    p0,
                    0.002,
                    0.95,
                    cross,
                    inst
            );

            System.out.println("RUNNING: " + cfg.tag());
            MainEA.run(cfg);
        }
    }
}
