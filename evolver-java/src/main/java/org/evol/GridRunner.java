package org.evol;

import java.util.List;

public class GridRunner {

    public static void main(String[] args) {

        List<Integer> seeds = List.of(1, 2, 3);
        List<Integer> popSizes = List.of(50, 100);
        List<Double> crossProbs = List.of(0.25, 0.5, 0.75);
        List<Double> p0s = List.of(0.002, 0.02 , 0.05);
        List<RPGInstance> instances = List.of(
				RPGInstance.EXPONENTIAL,
                RPGInstance.PIECEWISE,
                RPGInstance.BALANCED
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
                    0.90,
                    cross,
                    inst
            );

            System.out.println("RUNNING: " + cfg.tag());
            MainEA.run(cfg);
        }
    }
}
